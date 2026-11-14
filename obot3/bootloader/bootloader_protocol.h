#pragma once
#include <stdint.h>

// How a host loads flash through the obot3 can bootloader. Requests are can fd
// frames on the node's obot command id (1 << 7 | address); each gets exactly
// one reply on its status id (3 << 7 | address), sent when the request is
// done, so a host sends the next request when the reply is in. A reply repeats
// the command, with BOOTLOADER_REPLY set, and the sequence number of its
// request. Words are little endian.
//
//   command  request                         reply
//   INFO     -                               value: protocol version, data: a BootloaderInfo
//   ERASE    address, data: uint32 length    the pages that overlap the range are erased, unless
//                                            they are blank already
//   WRITE    address, length, data           length bytes are written and compared. The address is a
//                                            multiple of 8 and the pages are erased. A length that is
//                                            not a multiple of 8 is filled up with 0xFF.
//   READ     address, length                 length, data
//   CRC      address, data: uint32 length    value: the crc32 (zlib) of the range
//   RESET    -                               the node resets once the reply is out, into the application
//                                            if there is one
//   ENTER    -                               nothing here. An application takes this one request, as an
//                                            8 byte frame, and resets into the bootloader once its
//                                            reply is out: the way in that needs no text api
//
// ERASE and WRITE take ranges in the application's flash, from
// BootloaderInfo.application_address to flash_end; READ and CRC anywhere in
// the flash. On an error the reply has a status other than BOOTLOADER_OK and,
// for BOOTLOADER_FLASH_ERROR, the error flags of the flash status register as
// its value. A WRITE can be repeated: data that is already there is not an
// error. An erase takes about 25 ms per page, a full WRITE 0.6 ms.
//
// The node holds three requests, so a host may send a WRITE while the reply
// to the one before is still out, which keeps the flash busy all the time.
//
// The commands of the earlier bootloader, 0 and 1 without replies, are not
// accepted; it in turn ignores the commands here, so a host can tell the two
// apart with an INFO request.

enum {
    BOOTLOADER_INFO = 0x10,
    BOOTLOADER_ERASE = 0x11,
    BOOTLOADER_WRITE = 0x12,
    BOOTLOADER_READ = 0x13,
    BOOTLOADER_CRC = 0x14,
    BOOTLOADER_RESET = 0x15,
    BOOTLOADER_ENTER = 0x16,
    BOOTLOADER_REPLY = 0x80,
};

enum {
    BOOTLOADER_OK,
    BOOTLOADER_BAD_COMMAND,
    BOOTLOADER_BAD_RANGE,       // address or length outside of what the command takes
    BOOTLOADER_FLASH_ERROR,
    BOOTLOADER_VERIFY_ERROR,    // the flash does not read back as written
};

enum { BOOTLOADER_PROTOCOL_VERSION = 1 };

struct BootloaderRequest {
    uint8_t command;
    uint8_t sequence;           // comes back in the reply
    uint8_t length;             // bytes of data, WRITE and READ
    uint8_t reserved;
    uint32_t address;
    uint8_t data[56];
};

struct BootloaderReply {
    uint8_t command;
    uint8_t sequence;
    uint8_t status;
    uint8_t length;             // bytes of data
    uint32_t value;
    uint8_t data[56];
};

struct BootloaderInfo {
    uint32_t application_address;
    uint32_t flash_end;
    uint32_t page_size;
    uint32_t max_data;          // bytes of data in a WRITE or READ
};
