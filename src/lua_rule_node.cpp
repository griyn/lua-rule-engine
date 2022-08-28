#include "lua_rule_node.h"

#include <iostream>
#include "lua_functions.h"

namespace garden {

int LuaRuleNode::init(const RuleConfig& rule_config) {
	_name = rule_config.name();

	// 把config.expr包装成可直接执行的样子
	_lua_function = "function " + _name + "()\n" + "return " +
            rule_config.expr() + "\nend";

	if (_lua_engine.init(_lua_function) != 0) {
        std::cerr << "lua_engine init error, name:" << _name << std::endl;
        return -1;
    }

    if (!_lua_engine.is_function_exist(_name)) {
        std::cerr << "lua_engine is_function_exist err, name: " << std::endl;
        return -1;
    }

	for (int i = 0; i < rule_config.nexts_size(); ++i) {
		_nexts.emplace(rule_config.nexts(i));
	}	

	return 0;
}

bool LuaRuleNode::run_rule(::google::protobuf::Message* message) {
	bool res = false;
    if (_lua_engine.call_lua_bool_function(_name, &res, message) != 0) {
        std::cerr << "lua engine call function failed, name:" << _name
                 << " lua_function:" << _lua_function << std::endl;
        return false;
    }

    return res;
}

} // namespace garden
