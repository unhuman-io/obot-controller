#! /usr/bin/env python3

import argparse
import time
import struct
import subprocess

class can_loader:
    def __init__(self, can_id, address, file):
        self.can_id = can_id
        self.address = address
        self.file = file

    def load(self):
        print(f"Loading file: {self.file.name} for CAN ID: {self.can_id} at address: {self.address}")
        with open(self.file.name, 'rb') as f:
            data = f.read()
            
            print(f"Data length to load: {len(data)} bytes")
            for i in range(0, len(data), 32):
                chunk = data[i:i+32]
                address = self.address + i
                print(f"Writing to address {address:02x}: {chunk.hex()}")
                address_le = struct.unpack('<I', struct.pack('>I', address))[0]
                write_str = f"08{self.can_id:1x}##101{len(chunk):02x}0000{address_le:08x}" + ''.join(f"{b:02x}" for b in chunk)
                print(write_str)
                subprocess.run(['cansend', 'can0', write_str], check=True)
                if i % 0x800 == 0:
                    # max erase time 24.47 ms
                    print(f"Waiting for 30 milliseconds after writing {address:02x}...")
                    time.sleep(.03)
                # write time 32 bytes 320 us
                time.sleep(.0003)

                

# example usage:
# ../scripts/load_can.py --can_id 2 --address 0x8002000 --file ../obot_g474/build/tmotor_driver/tmotor_driver_noparam.bin
# ../scripts/load_can.py --can_id 2 --address 0x8060000 --file ../obot_g474/build/tmotor_driver/tmotor_driver_param_only.bin


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Load CAN configuration for the robot controller.")
    parser.add_argument("--can_id", type=int, required=True, help="CAN ID to set for the controller.")
    parser.add_argument("--address", type=lambda x: int(x,0), required=True, help="Load address")
    parser.add_argument("--file", type=argparse.FileType('r'), required=True, help="File to load the CAN configuration from.")
    args = parser.parse_args()

    # Load the CAN configuration
    can_id = args.can_id
    address = args.address
    file = args.file
    can_loader_instance = can_loader(can_id, address, file)
    can_loader_instance.load()

