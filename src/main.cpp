#include "lua_engine.h"
#include "lua_rule_node.h"
#include <iostream>


int main() {
	garden::LuaRuleNode node;
	garden::RuleConfig config;
	config.set_name("test1");
	config.set_expr("get_pb_int('n') == 1");
	config.set_n(1);

	if (node.init(config) != 0) {
		std::cerr << "node init error" << std::endl;
		return -1;
	}

	bool ret = node.run_rule(&config);
	std::cout << "run_rule:" << ret << std::endl;

	config.set_n(2);
	ret = node.run_rule(&config);
	std::cout << "run_rule:" << ret << std::endl;

	return 0;
}
