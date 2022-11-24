#pragma once

#include "lua_engine.h"
#include "node.h"
#include "lua_rule_engine_config.pb.h"
#include <set>

namespace garden {

class LuaRuleNode : public Node {
public:
    virtual int init(const RuleConfig& rule_config) override;

    // 执行规则, 返回执行成功/失败。输入统一为proto
    virtual bool run(::google::protobuf::Message* message) override;

    const std::set<std::string>& nexts() override;

private:
    std::string _name;
    std::string _lua_function;
    LuaEngine _lua_engine;
    std::set<std::string> _nexts;
};

} // namespace garden