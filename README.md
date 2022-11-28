# LuaRuleEngine
lua规则引擎。

输入数据 - 满足规则 - do somethiing - 输出数据。

## Feature
* 以protobuf Message为输入输出的数据结构。Message可以是任何业务实现的结构，free schema。
* 支持热加载。lua作为解释型语言可以无需编译，加载到内存马上使用。
* 节点可以在不同线程并发执行，没有竞争。
* 支持多个节点组成规则链，按顺序执行。（doing）

## Install Lua
> https://www.lua.org/download.html
```
curl -R -O http://www.lua.org/ftp/lua-5.4.4.tar.gz
tar zxf lua-5.4.4.tar.gz
cd lua-5.4.4
make linux test
make install
```

## Srcouce file
```
* lua_rule_node: 上层接口，直接调用方法初始化、执行规则、获取下一个节点，以proto为输入数据。
* lua_engine: 封装了lua函数的注册和调用。
* lua_functions: 定义了lua可以调用的c函数，大多与proto相关，用lua全局变量传递参数。
```
