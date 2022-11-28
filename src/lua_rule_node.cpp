#include "lua_rule_node.h"

#include <iostream>
#include "lua_functions.h"

namespace garden {

// 对外提供的查词典方法，可以在lua表达式中使用
std::string LuaRuleNode::_find_in_dict {
        "function find_in_dict(ele, dict)\n"
            "return dict[ele] == true\n"
        "end" }; 

// 把配置中的list组装成可查询的table结构
std::string LuaRuleNode::_to_dict {
        "function to_dict(list)\n"
            "local set = {}\n"
            "for _, l in ipairs(list) do set[l] = true end\n"
            "return set\n"
        "end"}; 

int LuaRuleNode::init(const RuleConfig& rule_config) {
    _name = rule_config.name();

    // 添加查询词典函数
    if (!_lua_engine.add_script(_find_in_dict) 
            || !_lua_engine.is_function_exist("find_in_dict")) {
        std::cerr << "lua_engine add_function error, name: find_in_dict";
        return -1;
    }

    if (!_lua_engine.add_script(_to_dict)
            || !_lua_engine.is_function_exist("to_dict")) {
        std::cerr << "lua_engine add_function error, name: to_dict";
        return -1;
    }

    // 添加词典
    for (int i = 0; i < rule_config.dicts_size(); ++i) {
        const auto& dict = rule_config.dicts(i);
        std::string dict_str = 
                "tmp_dict = " + dict.data() + "\n" 
                + dict.name() + "= to_dict(tmp_dict)\n"
                "tmp_dict = nil";
        if (!_lua_engine.add_script(dict_str)
                || !_lua_engine.is_table_exist(dict.name())) {
            std::cerr << "lua_engine add dict_str error, dict_str:" << dict_str << std::endl;
            return -1;
        }
    }

    // 添加节点函数
    // 把config.expr包装成可直接执行的样子
    _lua_function = "function " + _name + "()\n" + "return " +
        rule_config.expr() + "\nend";

    if (!_lua_engine.add_script(_lua_function)) {
        std::cerr << "lua_engine add_function error, name:" << _name << std::endl;
        return -1;
    }

    if (!_lua_engine.is_function_exist(_name)) {
        std::cerr << "lua_engine is_function_exist error, name:" << _name << std::endl;
        return -1;
    }

    for (int i = 0; i < rule_config.nexts_size(); ++i) {
        _nexts.emplace(rule_config.nexts(i));
    }

    return 0;
}

bool LuaRuleNode::run(::google::protobuf::Message* message) {
    bool res = false;
    if (_lua_engine.call_lua_bool_function(_name, &res, message) != 0) {
        std::cerr << "lua engine call function failed, name:" << _name
                 << " lua_function:" << _lua_function << std::endl;
        return false;
    }

    return res;
}

const std::set<std::string>& LuaRuleNode::nexts() {
    return _nexts;
}

} // namespace garden