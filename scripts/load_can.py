#! /usr/bin/env python3
"""Loads a binary into the flash of a node that is in its can bootloader.

With the obot3 bootloader (obot3/bootloader/bootloader_protocol.h) the range is
erased, written with a reply per frame, and compared by crc. The earlier
bootloader (motorlib/bootloader/stm32g4) does not answer: it gets the frames at
a rate it can keep up with, and nothing is checked.

A node that runs its application is sent into the bootloader first, and the
node is reset after the load, which starts what was loaded. With --stay it is
left in the bootloader. A node says what it runs in its reply to an obot enum
request and takes the bootloader's ENTER request, so none of this needs the
text api; firmware from before that is asked and sent there by text.

example usage:
  load_can.py --can_id 2 --address 0x8002000 --file build/gcc/obot3/obot3_tmotor_driver.bin
  load_can.py --can_id 2 --address 0x8060000 --file tmotor_driver_param.bin
"""

import argparse
import socket
import struct
import sys
import time
import zlib

CAN_RAW_FD_FRAMES = getattr(socket, "CAN_RAW_FD_FRAMES", 5)
CANFD_BRS = 1
CANFD_FRAME = struct.Struct("<IBB2x64s")

# obot can ids: message << 7 | address
OBOT_COMMAND, OBOT_STATUS, OBOT_ASCII, OBOT_ASCII_RESPONSE, OBOT_ENUM = 1, 3, 4, 5, 15
ENUM_RESPONSE = struct.Struct("<2s6s2sHI20s8sBxH")    # motor_messages.h EnumResponse
BOARD_RESET = 255   # MotorCommand mode_desired

INFO, ERASE, WRITE, READ, CRC, RESET, ENTER = 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16
REPLY = 0x80
STATUS = {1: "bad command", 2: "bad range", 3: "flash error", 4: "verify error"}
ERASE_SECONDS_PER_PAGE = 0.05   # 25 ms typical


class BootloaderError(Exception):
    pass


class NoReply(BootloaderError):
    pass


class Bootloader:
    def __init__(self, can_id, interface="can0"):
        self.request_id = OBOT_COMMAND << 7 | can_id
        self.reply_id = OBOT_STATUS << 7 | can_id
        self.text_id = OBOT_ASCII << 7 | can_id
        self.text_reply_id = OBOT_ASCII_RESPONSE << 7 | can_id
        self.enum_id = OBOT_ENUM << 7 | can_id
        self.sequence = 0
        self.socket = socket.socket(socket.AF_CAN, socket.SOCK_RAW, socket.CAN_RAW)
        self.socket.setsockopt(socket.SOL_CAN_RAW, CAN_RAW_FD_FRAMES, 1)
        self.socket.setsockopt(socket.SOL_CAN_RAW, socket.CAN_RAW_FILTER,
                               struct.pack("<IIIIII", self.reply_id, 0x7FF, self.text_reply_id, 0x7FF, self.enum_id, 0x7FF))
        self.socket.bind((interface,))

    def send(self, payload, can_id=None):
        self.socket.send(CANFD_FRAME.pack(self.request_id if can_id is None else can_id, len(payload), CANFD_BRS, payload))

    def receive(self, can_id, timeout):
        """The next frame with this id: its data, or None after the timeout."""
        deadline = time.monotonic() + timeout
        while (remaining := deadline - time.monotonic()) > 0:
            self.socket.settimeout(remaining)
            try:
                frame = self.socket.recv(CANFD_FRAME.size)
            except socket.timeout:
                break
            frame_id, length, _, data = CANFD_FRAME.unpack(frame.ljust(CANFD_FRAME.size, b"\0"))
            if frame_id & 0x7FF == can_id:
                return data[:length]
        return None

    def text(self, request, timeout=0.1):
        """The obot text api, which applications and bootloaders answer: the reply, or None without one."""
        self.send(request.encode() + b"\0", self.text_id)
        reply = self.receive(self.text_reply_id, timeout)
        return None if reply is None else reply.split(b"\0")[0].decode(errors="replace")

    def enum(self, timeout=0.1):
        """The node's EnumResponse as a tuple, or None from a node that has none."""
        self.send(b"", self.enum_id)
        reply = self.receive(self.enum_id, timeout)
        if reply is None or len(reply) < ENUM_RESPONSE.size:
            return None
        return ENUM_RESPONSE.unpack_from(reply)

    def name(self, timeout=0.1, tries=3):
        for _ in range(tries):
            if (enum := self.enum(timeout)) is not None:
                return enum[5].split(b"\0")[0].decode(errors="replace")
            if (name := self.text("name", timeout)) is not None:
                return name
        return None

    def enter(self):
        """Gets a node that runs an application into its bootloader."""
        name = self.name()
        if name is None:
            raise BootloaderError("the node does not answer")
        if name == "bootloader":
            return
        print(f"node is '{name}': resetting it into the bootloader")
        try:
            self.request(ENTER, tries=1)                                # obot3 applications
        except NoReply:
            self.text("go_to_bootloader=b007")                          # those from before ENTER
            self.text("reset")
            self.send(struct.pack("<IB59x", 0, BOARD_RESET))            # obot_g474 applications
        deadline = time.monotonic() + 3
        while time.monotonic() < deadline:
            if self.name(tries=1) == "bootloader":
                return
        raise BootloaderError(f"the node did not get to its bootloader, it is '{self.name()}'")

    def run(self):
        """Resets the node, which starts the application. Returns its name then."""
        try:
            self.request(RESET, tries=1)
        except NoReply:
            self.text("reset")      # the earlier bootloader, which doesn't reply to it either
        time.sleep(0.3)
        return self.name()

    def start(self, command, address=0, length=0, data=b""):
        """Sends a request. What it returns is for reply()."""
        self.sequence = (self.sequence + 1) & 0xFF
        payload = struct.pack("<BBBxI", command, self.sequence, length, address) + data
        self.send(payload)
        return command, self.sequence, address, payload

    def reply(self, request, timeout=0.1, tries=3):
        """The reply to a request: (value, data). The request is repeated if no reply comes."""
        command, request_sequence, address, payload = request
        for attempt in range(tries):
            if attempt != 0:
                self.send(payload)
            deadline = time.monotonic() + timeout
            while (remaining := deadline - time.monotonic()) > 0:
                reply = self.receive(self.reply_id, remaining)
                if reply is None or len(reply) < 8:
                    continue
                reply_command, sequence, status, reply_length, value = struct.unpack_from("<BBBBI", reply)
                if reply_command != command | REPLY or sequence != request_sequence:
                    continue    # the reply to something earlier
                if status != 0:
                    raise BootloaderError(f"{STATUS.get(status, status)} on command 0x{command:02x} "
                                          f"at 0x{address:08x}, value 0x{value:x}")
                return value, reply[8:8 + reply_length]
        raise NoReply(f"no reply to command 0x{command:02x} at 0x{address:08x}")

    def request(self, command, address=0, length=0, data=b"", timeout=0.1, tries=3):
        """One request and its reply: (value, data)."""
        return self.reply(self.start(command, address, length, data), timeout, tries)

    def info(self):
        version, data = self.request(INFO, tries=2, timeout=0.05)
        application_address, flash_end, page_size, max_data = struct.unpack("<IIII", data[:16])
        return version, application_address, flash_end, page_size, max_data

    def load(self, address, data, verbose=False):
        version, application_address, flash_end, page_size, max_data = self.info()
        if verbose:
            print(f"protocol {version}, application flash 0x{application_address:08x} to 0x{flash_end:08x}, "
                  f"{page_size} byte pages")
        t_start = time.monotonic()
        erase_step = 8 * page_size
        for offset in range(0, len(data), erase_step):
            length = min(erase_step, len(data) - offset)
            pages = length // page_size + 2
            self.request(ERASE, address + offset, data=struct.pack("<I", length), timeout=pages * ERASE_SECONDS_PER_PAGE)
            print(".", end="", flush=True)
        t_erased = time.monotonic()
        step = max_data - max_data % 8
        # One write ahead of the replies: the node takes the next frame in while it programs
        previous = None
        for offset in range(0, len(data), step):
            chunk = data[offset:offset + step]
            request = self.start(WRITE, address + offset, len(chunk), chunk)
            if previous is not None:
                self.reply(previous)
            previous = request
        if previous is not None:
            self.reply(previous)
        t_written = time.monotonic()
        crc, _ = self.request(CRC, address, data=struct.pack("<I", len(data)), timeout=1)
        print("")
        if crc != zlib.crc32(data):
            raise BootloaderError(f"crc 0x{crc:08x} of the flash is not the file's, 0x{zlib.crc32(data):08x}")
        print(f"erased in {t_erased - t_start:.2f} s, written in {t_written - t_erased:.2f} s "
              f"({len(data) / (t_written - t_erased) / 1000:.0f} kB/s), crc 0x{crc:08x} ok")

    def load_legacy(self, address, data, verbose=False):
        """The earlier bootloader: write frames of 32 bytes, with time for the erase at each page."""
        for offset in range(0, len(data), 32):
            chunk = data[offset:offset + 32]
            if verbose:
                print(f"Writing to address {address + offset:02x}: {chunk.hex()}")
            self.send(struct.pack("<BBxxI", 1, len(chunk), address + offset) + chunk)
            if offset % 0x800 == 0:
                if not verbose:
                    if offset != 0 and offset % 0x10000 == 0:
                        print("")
                    print(".", end="", flush=True)
                time.sleep(.03)     # max erase time 24.47 ms
            time.sleep(.0006)       # write time 32 bytes 320 us
        print("")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Load a binary through the can bootloader.")
    parser.add_argument("--can_id", type=int, required=True, help="CAN ID of the controller.")
    parser.add_argument("--address", type=lambda x: int(x, 0), required=True, help="Load address")
    parser.add_argument("--file", type=argparse.FileType('rb'), required=True, help="Binary file to load.")
    parser.add_argument("--interface", default="can0", help="CAN interface.")
    parser.add_argument("--verbose", action="store_true", help="Enable verbose output.")
    parser.add_argument("--stay", action="store_true", help="Leave the node in the bootloader instead of resetting it after the load.")
    args = parser.parse_args()

    data = args.file.read()
    print(f"Loading file: {args.file.name} for CAN ID: {args.can_id} at address: 0x{args.address:07x}")
    print(f"Data length to load: {len(data)} bytes")
    bootloader = Bootloader(args.can_id, args.interface)
    try:
        bootloader.enter()
        try:
            bootloader.info()
        except NoReply:
            print("no reply to the info request: loading as for the earlier bootloader, unchecked")
            bootloader.load_legacy(args.address, data, args.verbose)
        else:
            bootloader.load(args.address, data, args.verbose)
        if not args.stay:
            name = bootloader.run()
            print("node does not answer after the reset" if name is None else f"node is '{name}' after the reset")
    except BootloaderError as error:
        print(f"\nfailed: {error}")
        sys.exit(1)
