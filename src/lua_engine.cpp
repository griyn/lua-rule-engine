#include "lua_engine.h"

namespace garden {

LuaEngine::LuaEngine() {
    _lua = luaL_newstate(); // 创建一个status，返回指向lua堆栈的指针
}

LuaEngine::~LuaEngine() {
    if (_lua != nullptr) {
        lua_close(_lua);
    }
}

int LuaEngine::init(const std::string& script) {
    // 1. 打开lua的所有标准库
    luaL_openlibs(_lua);

    // 2. 注册提供的C++函数
	/*lua_register(_lua, "get_pb_int", lua_functions::get_pb_int);
    lua_register(_lua, "get_pb_string", lua_functions::get_pb_string);
    lua_register(_lua, "get_ctrl_int", lua_functions::get_ctrl_int);
    lua_register(_lua, "get_ctrl_string", lua_functions::get_ctrl_string);*/

	// 3. 加载lua脚本 & 检查语法. 这里定义的lua函数不在脚本中定义的话不会被执行，类似sh
	if (luaL_dostring(_lua, script.c_str()) != 0)  {
        lua_pop(_lua, 1);
        return -1;
    }

	return 0;
}

bool LuaEngine::is_function_exist(const std::string& f) {
    lua_getglobal(_lua, f.c_str());
    bool ret = lua_isfunction(_lua, lua_gettop(_lua));
    lua_pop(_lua, 1);
    return ret;
}

int LuaEngine::call_lua_bool_function(const std::string& f, bool* res) {
    lua_getglobal(_lua, f.c_str());

    // 1.调用指定名字的lua函数，指定没有参数，有1个返回值
    if (lua_pcall(_lua, 0, 1, 0) != 0) {
        lua_pop(_lua, 1);
        return -1;
    }

    // 2.判断返回值是否会bool
    if (!lua_isboolean(_lua, -1)) {
        return -1;
    }

    // 3.获取返回值
    *res = lua_toboolean(_lua, -1);
    lua_pop(_lua, 1);
    return 0;
}

} // garden
