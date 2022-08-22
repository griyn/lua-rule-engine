#include "lua_engine.h"
#include <iostream>

int main() {
	garden::LuaEngine lua_engine;
	int r = lua_engine.init("function test()\n return 1 == 1\n end");
	std::cout << r << std::endl;
	r = lua_engine.is_function_exist("test");
	std::cout << r << std::endl;
	bool br = true;
	r = lua_engine.call_lua_bool_function("test", &br);
	std::cout << r << " " << br << std::endl;
	return 0;
}
