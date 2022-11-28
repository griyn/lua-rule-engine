#include "lua_engine.h"

#include "lua_functions.h"

namespace garden {

LuaEngine::LuaEngine() {
    // 创建一个status，返回指向lua堆栈的指针
    _lua = luaL_newstate();

    // 打开lua的所有标准库
    luaL_openlibs(_lua);

    // 注册提供的C++函数
    lua_register(_lua, "get_pb_int", lua_functions::get_pb_int);
    lua_register(_lua, "get_pb_string", lua_functions::get_pb_string);
}

LuaEngine::~LuaEngine() {
    if (_lua != nullptr) {
        lua_close(_lua);
    }
}

bool LuaEngine::add_script(const std::string& script) {
    // 加载lua脚本 & 检查语法. 这里定义的lua函数不在脚本中定义的话不会被执行，类似sh
    if (luaL_dostring(_lua, script.c_str()) != 0)  {
        std::cerr << "luaL_dostring error:" << lua_tostring(_lua, -1) << std::endl;
        lua_pop(_lua, 1); // pop error message
        return false;
    }

    return true;
}

bool LuaEngine::is_function_exist(const std::string& f) {
    lua_getglobal(_lua, f.c_str());
    bool ret = lua_isfunction(_lua, lua_gettop(_lua));
    lua_pop(_lua, 1);
    return ret;
}

bool LuaEngine::is_table_exist(const std::string& g) {
    lua_getglobal(_lua, g.c_str());
    bool ret = lua_istable(_lua, lua_gettop(_lua));
    lua_pop(_lua, 1);
    return ret;
}

int LuaEngine::call_lua_bool_function(const std::string& f, bool* res, void* p) {
    // 获得函数
    lua_getglobal(_lua, f.c_str());

    // 以全局变量传参
    lua_pushlightuserdata(_lua, p);
    lua_setglobal(_lua, "p"); // 出栈第一个元素并设定名称成为全局变量

    // 1.调用指定名字的lua函数，指定没有参数，有1个返回值
    if (lua_pcall(_lua, 0, 1, 0) != 0) {
        std::cerr << "lua_pcall failed:" << lua_tostring(_lua, 1) << std::endl;
        lua_pop(_lua, 1);
        return -1;
    }

    // 2.判断返回值是否会bool
    if (!lua_isboolean(_lua, -1)) {
        std::cerr << "lua_pcall return not bool" << std::endl;
        return -1;
    }

    // 3.获取返回值
    *res = lua_toboolean(_lua, -1);
    lua_pop(_lua, 1);
    return 0;
}

} // garden
