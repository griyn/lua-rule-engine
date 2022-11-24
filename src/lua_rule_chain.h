#include <string>
#include <mutex>
#include "lua_rule_node.h"

namespace garden {

// 由于lua栈不能多线程并发调用，chian使用tls单例的方式使用

class LuaRuleChain {
public:
    virtual ~LuaRuleChain() {}
    LuaRuleChain(const LuaRuleChain&) = delete;
    LuaRuleChain(LuaRuleChain&&) = delete;

    static void set_conf(const std::string& conf);

    static LuaRuleChain* get_tls_instance();

    void run_rules();

private:
    LuaRuleChain();

    int init();

private:
    // 所有tls实例的访问入口，用于热加载
    class Accessor {
    public:
        void add(LuaRuleChain* ins);
        bool modify_all();
    private:
        std::mutex _mutex;
        std::vector<LuaRuleChain*> _ins_vec;
    };

private:
    static std::string _conf;
    static thread_local LuaRuleChain _instance; // tls实例
    static thread_local bool _is_initialized; // 是否已初始化
};

} // namespace garden