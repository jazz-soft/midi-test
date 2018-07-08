#include <node_api.h>
#include <vector>
#include <iostream>
#include "miditest.h"

#define XX(x) if (x) return 0;

namespace miditest {

napi_ref MidiSrc_ctor;
napi_ref MidiDst_ctor;


std::string read_utf8(napi_env env, napi_value obj)
{
    napi_value value;
    if (napi_coerce_to_string(env, obj, &value)) return "";
    std::string str;
    size_t sz = 16;
    size_t len = 16;
    while (sz - len < 8) { // consider utf8 surrogate pairs
        sz *= 2;
        char* buf = new char[sz];
        if (napi_get_value_string_utf8(env, value, buf, sz, &len)) return "";
        str = buf;
        delete[] buf;
    }
    return str;
}


void destroy(napi_env env, void* data, void* hint) { std::cout << "delete!\n"; delete (CMidi*)data; }


napi_value name(napi_env env, napi_callback_info args)
{
    napi_value self;
    CMidi* ptr;
    XX (napi_get_cb_info(env, args, 0, 0, &self, 0));
    XX (napi_unwrap(env, self, (void**)&ptr));
    napi_value value;
    XX (napi_create_string_utf8(env, ptr->name().c_str(), ptr->name().length(), &value));
    return value;
}


napi_value connected(napi_env env, napi_callback_info args)
{
    napi_value self;
    CMidi* ptr;
    XX (napi_get_cb_info(env, args, 0, 0, &self, 0));
    XX (napi_unwrap(env, self, (void**)&ptr));
    napi_value value;
    XX (napi_get_boolean(env, ptr->connected(), &value));
    return value;
}


napi_value connect(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_value value;
    CMidi* ptr;
    XX (napi_get_cb_info(env, args, 0, 0, &self, 0));
    XX (napi_unwrap(env, self, (void**)&ptr));
    XX (napi_get_boolean(env, ptr->connect(), &value));
    return value;
}


napi_value disconnect(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_value value;
    CMidi* ptr;
    XX (napi_get_cb_info(env, args, 0, 0, &self, 0));
    XX (napi_unwrap(env, self, (void**)&ptr));
    XX (napi_get_boolean(env, ptr->disconnect(), &value));
    return value;
}


napi_value emit(napi_env env, napi_callback_info args)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_value self;
    napi_value value;
    napi_value number;
    bool bbb;
    CMidiSrc* ptr;
    uint32_t len;
    uint32_t num;
    XX (napi_get_cb_info(env, args, &argc, argv, &self, 0));
    XX (napi_is_array(env, argv[0], &bbb));
    if (!bbb) {
        napi_throw_type_error(env, "MIDI message", "expected Array");
        return 0;
    }
    XX (napi_get_array_length(env, argv[0], &len));
    if (!len) {
        XX (napi_get_boolean(env, false, &value));
        return value;
    }
    std::vector<unsigned char> msg;
    for (size_t i = 0; i < len; i++) {
        XX (napi_get_element(env, argv[0], i, &value));
        if (napi_coerce_to_number(env, value, &number) || napi_get_value_uint32(env, number, &num)) {
            napi_throw_type_error(env, "MIDI message", "expected 1-Byte values");
            return 0;
        }
        if (num > 255) {
            napi_throw_range_error(env, "MIDI message", "expected 1-Byte values");
            return 0;
        }
        msg.push_back(num);
    }
    XX (napi_unwrap(env, self, (void**)&ptr));
    XX (napi_get_boolean(env, ptr->emit(msg), &value));
    return value;
}


napi_value MidiSrc(napi_env env, napi_callback_info args)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_value self;
    napi_value value;
    bool bbb;
    XX (napi_get_cb_info(env, args, &argc, argv, &self, 0));
    XX (napi_get_reference_value(env, MidiSrc_ctor, &value));
    XX (napi_instanceof(env, self, value, &bbb));
    if (!bbb) {
        XX (napi_new_instance(env, value, argc, argv, &self));
        return self;
    }
    std::string str = read_utf8(env, argv[0]);
    CMidiSrc* Src = CMidi::CreateSrc(str);
    XX (napi_wrap(env, self, Src, destroy, 0, 0));
    return self;
}


napi_value MidiDst(napi_env env, napi_callback_info args)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_value self;
    napi_value value;
    bool bbb;
    XX (napi_get_cb_info(env, args, &argc, argv, &self, 0));
    XX (napi_get_reference_value(env, MidiDst_ctor, &value));
    XX (napi_instanceof(env, self, value, &bbb));
    if (!bbb) {
        XX (napi_new_instance(env, value, argc, argv, &self));
        return self;
    }
    std::string str = read_utf8(env, argv[0]);
    CMidiDst* Dst = CMidi::CreateDst(str);
    XX (napi_wrap(env, self, Dst, destroy, 0, 0));
    return self;
}


void MidiCallback(void* obj, const std::vector<unsigned char>& msg)
{
    std::cout << "MIDI Callback:";
    for (size_t i = 0; i < msg.size(); i++) std::cout << " " << (int)msg[i];
    std::cout << "\n";
}


napi_value init(napi_env env, napi_value exports)
{
    napi_value ctor;

    napi_property_descriptor Src[] = {
        { "connect", 0, connect, 0, 0, 0, napi_enumerable, 0 },
        { "disconnect", 0, disconnect, 0, 0, 0, napi_enumerable, 0 },
        { "connected", 0, 0, connected, 0, 0, napi_enumerable, 0 },
        { "name", 0, 0, name, 0, 0, napi_enumerable, 0 },
        { "emit", 0, emit, 0, 0, 0, napi_enumerable, 0 }
    };
    napi_property_descriptor Dst[] = {
        { "connect", 0, connect, 0, 0, 0, napi_enumerable, 0 },
        { "disconnect", 0, disconnect, 0, 0, 0, napi_enumerable, 0 },
        { "connected", 0, 0, connected, 0, 0, napi_enumerable, 0 },
        { "name", 0, 0, name, 0, 0, napi_enumerable, 0 }
    };

    XX (napi_define_class(env, "MidiSrc", NAPI_AUTO_LENGTH, MidiSrc, 0, sizeof Src / sizeof Src[0], Src, &ctor));
    XX (napi_create_reference(env, ctor, 1, &MidiSrc_ctor));
    XX (napi_set_named_property(env, exports, "MidiSrc", ctor));

    XX (napi_define_class(env, "MidiDst", NAPI_AUTO_LENGTH, MidiDst, 0, sizeof Dst / sizeof Dst[0], Dst, &ctor));
    XX (napi_create_reference(env, ctor, 1, &MidiDst_ctor));
    XX (napi_set_named_property(env, exports, "MidiDst", ctor));

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace miditest
