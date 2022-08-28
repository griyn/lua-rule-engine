#pragma once

#include "google/protobuf/message.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace garden {
namespace lua_functions {

int get_pb_int(lua_State* L);

int get_pb_string(lua_State* L);

bool get_message_field(
        const std::string& field,
        const google::protobuf::Message& message,
        std::string* value);

} // namespace lua_functions
} // namespace garden
