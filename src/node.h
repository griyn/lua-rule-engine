#pragma once

#include "lua_rule_engine_config.pb.h"

namespace garden {

class Node {
public:
    virtual int init(const RuleConfig& rule_config) = 0;

    // 执行规则, 返回执行成功/失败。输入统一为proto
    virtual bool run(::google::protobuf::Message* message) = 0;

    virtual const std::set<std::string>& nexts() = 0;
};

} // namespace garden