# LuaRuleEngine
lua规则引擎

## Download Lua
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

## TODO
多线程