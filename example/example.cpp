#include "lua_engine.h"
#include "lua_rule_node.h"
#include <iostream>
#include "person.pb.h"

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
        d->set_data("{'Tom', 'Jerry', 5}"); // 词典里可以同时添加字符和数字，因为在lua里变量没有类型，只有值有类型
        config.set_expr("find_in_dict(get_pb_string('name'), name_dict) and find_in_dict(get_pb_int('age'), name_dict)");

        if (node.init(config) != 0) {
            std::cerr << "node init error" << std::endl;
            return -1;
        }


        Person person;
        person.set_name("John");
        person.set_age(5);

        bool ret = node.run(&person);
        std::cout << "run_rule:" << ret << std::endl;

        person.set_name("Jerry");
        ret = node.run(&person);
        std::cout << "run_rule:" << ret << std::endl;
    }

    return 0;
}
