#pragma once
#include <cstdint>
#include <cstring>
#include "../motorlib/motor_messages/motor_messages.h"

// The reply to an obot enum request that goes to this node alone, see
// EnumResponse in motor_messages.h: it is all a host needs to list a node, so
// a node is found, and told from its bootloader, without the text api. To the
// request for all nodes the reply is empty: a host hears who is there, and
// asks each. The version is the build's git describe,
// v<major>.<minor>-<commits>-g<hash>, in numbers (obot3/CMakeLists.txt). No
// board is named here: those fields stay zero.
inline obot::EnumResponse obot_enum_response(const char* name, const uint8_t serial[6]) {
    obot::EnumResponse response{};
    response.messages_version[0] = MOTOR_MESSAGES_VERSION_MAJOR;
    response.messages_version[1] = MOTOR_MESSAGES_VERSION_MINOR;
    std::memcpy(response.serial_number, serial, sizeof(response.serial_number));
    response.version[0] = OBOT3_VERSION_MAJOR;
    response.version[1] = OBOT3_VERSION_MINOR;
    response.version_commits = OBOT3_VERSION_COMMITS;
    response.version_hash = OBOT3_VERSION_HASH | (OBOT3_VERSION_DIRTY ? ENUM_VERSION_DIRTY : 0);
    std::strncpy(response.name, name, sizeof(response.name));
    return response;
}
