#pragma once

#include <string>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace garden {

class LuaEngine {
public:
	LuaEngine();
    virtual ~LuaEngine();

    int init(const std::string& script);

	// 判断lua函数是否存在
    bool is_function_exist(const std::string& f);


    int call_lua_bool_function(const std::string& f, bool* res);	

private:
	// 一个lua环境，不能多线程并发运行。
	lua_State* _lua;
};

} // garden
