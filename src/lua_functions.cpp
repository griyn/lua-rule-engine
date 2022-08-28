#include "lua_functions.h"
#include <iostream>

namespace garden {
namespace lua_functions {

bool get_params(lua_State* L, std::string& name, google::protobuf::Message*& message) {
	if (L == nullptr) {
		return false;
	}

	// 检查参数数量和类型
	int argc = lua_gettop(L);
	if (argc != 1) {
		std::cerr << "lua param should 1, n:" << argc << std::endl;
		return false;
	}

	if (lua_isstring(L, -1) != 1) {
		std::cerr << "lua param should string" << std::endl;
		return false;
	}
	name.assign(lua_tostring(L, -1));

	lua_getglobal(L, "p");
	if (lua_islightuserdata(L, -1) != 1) {
		std::cerr << "lua global p should lightuserdata" << std::endl;
		return false;
	}
	message = static_cast<google::protobuf::Message*>(lua_touserdata(L, -1));

	return true;
}

// lua:1 - success; 0 - failed
int get_pb_int(lua_State* L) {
    std::string name;
	google::protobuf::Message* message;
	if (!get_params(L, name, message)) {
		std::cerr << "get params failed" << std::endl;
		return 0;
	}

	std::string v;
	if (!get_message_field(name, *message, &v)) {
		std::cerr << "get_message_field failed" << std::endl;
		return 0;
	}
	int64_t value = *reinterpret_cast<const int64_t*>(v.c_str());

	lua_pushinteger(L, value);
	return 1;
}

int get_pb_string(lua_State* L) {
    std::string name;
	google::protobuf::Message* message;
	if (!get_params(L, name, message)) {
		std::cerr << "get params failed" << std::endl;
		return 0;
	}

	std::string v;
	if (!get_message_field(name, *message, &v)) {
		std::cerr << "get_message_field failed" << std::endl;
		return 0;
	}

	lua_pushstring(L, v.c_str());
	return 1;
}

bool get_message_field(
        const std::string& field,
        const google::protobuf::Message& message,
        std::string* value) {
    auto* desc = message.GetDescriptor();
    auto* refl = message.GetReflection();

    static std::string s_token = ".";
    auto iter = field.find_first_of(s_token);
    // A.B.C 存在嵌套类型的情况
    if (iter != std::string::npos) {
        std::string cur = field.substr(0, iter);
        std::string oth = field.substr(iter + 1);
        auto nested_field_desc = desc->FindFieldByName(cur);
        if (nested_field_desc == nullptr) {
            std::cerr << "nested messge not found, field:" << cur
                    << " full_name:" << desc->full_name() << std::endl;
            return false;
        }
        if (nested_field_desc->is_repeated()) {
            std::cerr << "nested is repeated failed, field:" << cur
                    << " full_name:" << desc->full_name() << std::endl;
            return false;
        }
        if (nested_field_desc->type() != google::protobuf::FieldDescriptor::Type::TYPE_MESSAGE) {
            std::cerr << "nested is not message type failed, field:" << cur
                    << " full_name:" << desc->full_name() << std::endl;
            return false;
        }

        auto& nested_message = refl->GetMessage(message, nested_field_desc);

        return get_message_field(oth, nested_message, value);
    }

    auto* field_desc = desc->FindFieldByName(field);

    if (field_desc == nullptr) { // 没有这个字段
        std::cerr << "field not found, field:" << field << std::endl;
        return false;
    }

    if (field_desc->is_repeated()) {
        std::cerr << "do not support repeated: " << std::endl;
        return false;
    }

    switch (field_desc->cpp_type()) {
#define GET_PROTOBUF(PBTYPE, PBFUNC, CPPTYPE)                           \
    case google::protobuf::FieldDescriptor::CPPTYPE_##PBTYPE: {         \
        CPPTYPE tmp = refl->PBFUNC(message, field_desc);                \
        value->assign(reinterpret_cast<char*>(&tmp), sizeof(CPPTYPE));  \
        break;                                                          \
    }
        GET_PROTOBUF(DOUBLE, GetDouble, double  );
        GET_PROTOBUF(FLOAT , GetFloat , float   );
        GET_PROTOBUF(INT64 , GetInt64 , int64_t );
        GET_PROTOBUF(UINT64, GetUInt64, uint64_t);
        GET_PROTOBUF(INT32 , GetInt32 , int32_t );
        GET_PROTOBUF(UINT32, GetUInt32, uint32_t);
        GET_PROTOBUF(BOOL  , GetBool  , bool    );
#undef GET_PROTOBUF
    case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
        *value = refl->GetStringReference(message, field_desc, value);
        break;
    }
    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE: {
        std::cerr << "message type not support, field:" << field << std::endl;
        break;
    }

    default:
        std::cerr << "error type, field:" << field << std::endl;
        return false;
    }

    return true;
}

} // lua_functions
} // namespace garden
