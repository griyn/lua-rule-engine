#pragma once

#include "lua_engine.h"
#include "lua_rule_engine_config.pb.h"
#include <set>

namespace garden {

class LuaRuleNode {
public:
	int init(const RuleConfig& rule_config);

	// 执行规则, 返回执行成功/失败。输入统一为proto
	bool run_rule(::google::protobuf::Message* message);

private:
	std::string _name;
    std::string _lua_function;
    LuaEngine _lua_engine;
	std::set<std::string> _nexts;	
};

} // namespace garden
