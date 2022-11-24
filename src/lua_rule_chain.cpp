#include "lua_rule_chain.h"

namespace garden {

thread_local LuaRuleChain LuaRuleChain::_instance;
thread_local bool LuaRuleChain::_is_initialized = false;

void LuaRuleChain::set_conf(const std::string& conf) {
    _conf = conf;
}

LuaRuleChain* LuaRuleChain::get_tls_instance() {
    if (_is_initialized) {
        return &_instance;
    }

    if (_instance.init() != 0) {
        return nullptr; // 初始化失败
    }

    // 注册到accessor用于热加载
    _accessor.add(&_instance);

    _is_initialized = true; // 初始化成功
    return &_instance;
}

int init() {
    // 读配置成功
    return 0;
}

void run_rules() {
    // 执行chain rules
}

void LuaRuleChain::Accessor::add(LuaRuleChain* ins) {
    std::lock_guard<std::mutex> guard(_mutex);
    _ins_vec.push_back(ins);
}

bool LuaRuleChain::Accessor::modify_all() {
    std::lock_guard<std::mutex> guard(_mutex);
    for (auto& ins : _ins_vec) {
        // 重新通过conf加载实例。init的处理过程需要保证线程安全，解决方式dbd或者shared_ptr。
        if (ins->init() != 0) {
            return false;
        }
    }
    return true;
}

} // namespace garden