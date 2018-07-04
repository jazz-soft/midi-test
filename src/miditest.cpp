#include <node_api.h>
#include <iostream>
#include "miditest.h"

namespace miditest {

napi_ref MidiSrc_ctor;
napi_ref MidiDst_ctor;


std::string read_utf8(napi_env env, napi_value obj)
{
    napi_value value;
    napi_status status;
    status = napi_coerce_to_string(env, obj, &value);
    if (status != napi_ok) return "";
    std::string str;
    size_t sz = 16;
    size_t len = 16;
    while (sz - len < 8) { // consider utf8 surrogate pairs
        sz *= 2;
        char* buf = new char[sz];
        status = napi_get_value_string_utf8(env, value, buf, sz, &len);
        if (status != napi_ok) return "";
        str = buf;
        delete[] buf;
    }
    return str;
}


void destroy(napi_env env, void* data, void* hint) { std::cout << "delete hint!\n"; delete (CMidi*)data; }


napi_value name(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_status status;
    CMidi* ptr;
    status = napi_get_cb_info(env, args, 0, 0, &self, 0);
    if (status != napi_ok) return 0;
    status = napi_unwrap(env, self, (void**)&ptr);
    ptr->m_name;
    napi_value value;
    status = napi_create_string_utf8(env, ptr->m_name.c_str(), ptr->m_name.length(), &value);
    return value;
}


napi_value connected(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_status status;
    CMidi* ptr;
    status = napi_get_cb_info(env, args, 0, 0, &self, 0);
    if (status != napi_ok) return 0;
    status = napi_unwrap(env, self, (void**)&ptr);
    napi_value value;
    status = napi_get_boolean(env, ptr->m_connected, &value);
    return value;
}


napi_value connect(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_status status;
    CMidi* ptr;
    status = napi_get_cb_info(env, args, 0, 0, &self, 0);
    if (status != napi_ok) return 0;
    status = napi_unwrap(env, self, (void**)&ptr);
    bool result = ptr->connect();
    napi_value value;
    status = napi_get_boolean(env, result, &value);
    return value;
}


napi_value disconnect(napi_env env, napi_callback_info args)
{
    napi_value self;
    napi_status status;
    CMidi* ptr;
    status = napi_get_cb_info(env, args, 0, 0, &self, 0);
    if (status != napi_ok) return 0;
    status = napi_unwrap(env, self, (void**)&ptr);
    bool result = ptr->disconnect();
    napi_value value;
    status = napi_get_boolean(env, result, &value);
    return value;
}


napi_value MidiSrc(napi_env env, napi_callback_info args)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_value self;
    napi_value value;
    bool bbb;
    napi_status status;
    status = napi_get_cb_info(env, args, &argc, argv, &self, 0);
    if (status != napi_ok) return 0;

    status = napi_get_reference_value(env, MidiSrc_ctor, &value);
    if (status != napi_ok) return 0;

    status = napi_instanceof(env, self, value, &bbb);
    if (status != napi_ok) return 0;
    if (!bbb) {
        status = napi_new_instance(env, value, argc, argv, &self);
        return status == napi_ok ? self : 0;
    }

    std::string str = read_utf8(env, argv[0]);
    CSrc* Src = new CSrc(str);
    status = napi_wrap(env, self, Src, destroy, 0, 0);
    return status == napi_ok ? self : 0;
}


napi_value MidiDst(napi_env env, napi_callback_info args)
{
    size_t argc = 1;
    napi_value argv[1];
    napi_value self;
    napi_value value;
    bool bbb;
    napi_status status;
    status = napi_get_cb_info(env, args, &argc, argv, &self, 0);
    if (status != napi_ok) return 0;

    status = napi_get_reference_value(env, MidiDst_ctor, &value);
    if (status != napi_ok) return 0;

    status = napi_instanceof(env, self, value, &bbb);
    if (status != napi_ok) return 0;
    if (!bbb) {
        status = napi_new_instance(env, value, argc, argv, &self);
        return status == napi_ok ? self : 0;
    }

    std::string str = read_utf8(env, argv[0]);
    CDst* Dst = new CDst(str);
    status = napi_wrap(env, self, Dst, destroy, 0, 0);
    return status == napi_ok ? self : 0;
}


napi_value init(napi_env env, napi_value exports)
{
    napi_status status;
    napi_value ctor;

    napi_property_descriptor props[] = {
        { "connect", NULL, connect, NULL, NULL, NULL, napi_enumerable, NULL },
        { "disconnect", NULL, disconnect, NULL, NULL, NULL, napi_enumerable, NULL },
        { "connected", NULL, NULL, connected, NULL, NULL, napi_enumerable, NULL },
        { "name", NULL, NULL, name, NULL, NULL, napi_enumerable, NULL }
    };

    status = napi_define_class(env, "MidiSrc", NAPI_AUTO_LENGTH, MidiSrc, NULL, sizeof props / sizeof props[0], props, &ctor);
    if (status != napi_ok) return 0;
    status = napi_create_reference(env, ctor, 1, &MidiSrc_ctor);
    if (status != napi_ok) return 0;
    status = napi_set_named_property(env, exports, "MidiSrc", ctor);
    if (status != napi_ok) return 0;

    status = napi_define_class(env, "MidiDst", NAPI_AUTO_LENGTH, MidiDst, NULL, sizeof props / sizeof props[0], props, &ctor);
    if (status != napi_ok) return 0;
    status = napi_create_reference(env, ctor, 1, &MidiDst_ctor);
    if (status != napi_ok) return 0;
    status = napi_set_named_property(env, exports, "MidiDst", ctor);
    if (status != napi_ok) return 0;

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace miditest