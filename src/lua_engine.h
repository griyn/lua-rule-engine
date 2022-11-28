#pragma once

#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace garden {

// 对C++ lua api的封装
class LuaEngine {
public:
    LuaEngine();
    virtual ~LuaEngine();

    bool add_script(const std::string& script);

    // 判断lua函数是否存在
    bool is_function_exist(const std::string& f);

    // 判断lua全局变量是否存在
    bool is_table_exist(const std::string& g);

    int call_lua_bool_function(const std::string& f, bool* res, void* p);

private:
    // 一个lua环境，不能多线程并发运行。
    lua_State* _lua;
};

} // garden
