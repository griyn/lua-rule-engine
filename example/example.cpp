#include "lua_engine.h"
#include "lua_rule_node.h"
#include <iostream>


int main() {
    {
        garden::LuaRuleNode node;
        garden::RuleConfig config;
        config.set_name("test1");
        config.set_expr("get_pb_string('name') == 'test_name'");

        if (node.init(config) != 0) {
            std::cerr << "node init error" << std::endl;
            return -1;
        }

        bool ret = node.run(&config);
        std::cout << "run_rule:" << ret << std::endl;

        config.set_name("test_name");
        ret = node.run(&config);
        std::cout << "run_rule:" << ret << std::endl;
    }

    {
        garden::LuaRuleNode node;
        garden::RuleConfig config;
        config.set_name("test2");
        auto* d = config.add_dicts();
        d->set_name("name_dict");
        d->set_data("{'tom', 'jerry'}");
        config.set_expr("find_in_dict(get_pb_string('name'), name_dict)");

        if (node.init(config) != 0) {
            std::cerr << "node init error" << std::endl;
            return -1;
        }

        bool ret = node.run(&config);
        std::cout << "run_rule:" << ret << std::endl;

        config.set_name("jerry");
        ret = node.run(&config);
        std::cout << "run_rule:" << ret << std::endl;
    }

    return 0;
}
