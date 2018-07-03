#include <node_api.h>
#include <iostream>
#include "miditest.h"

namespace miditest {

napi_ref MidiSrc_ctor;
napi_ref MidiDst_ctor;

napi_value echo(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];
  napi_value self;
  napi_status status;
  status = napi_get_cb_info(env, args, &argc, argv, &self, 0);
  return argv[0];
}

napi_value MidiSrc(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];
  napi_value self;
  napi_value ctor;
  bool bbb;
  napi_status status;
  status = napi_get_cb_info(env, args, &argc, argv, &self, 0);
  if (status != napi_ok) return 0;

  status = napi_get_reference_value(env, MidiSrc_ctor, &ctor);
  if (status != napi_ok) return 0;

  status = napi_instanceof(env, self, ctor, &bbb);
  if (status != napi_ok) return 0;
  if (!bbb) {
    status = napi_new_instance(env, ctor, argc, argv, &self);
    return status == napi_ok ? self : 0;
  }

  return self;
}

napi_value MidiDst(napi_env env, napi_callback_info args) {
  size_t argc = 1;
  napi_value argv[1];
  napi_value self;
  napi_value ctor;
  bool bbb;
  napi_status status;
  status = napi_get_cb_info(env, args, &argc, argv, &self, 0);
  if (status != napi_ok) return 0;

  status = napi_get_reference_value(env, MidiDst_ctor, &ctor);
  if (status != napi_ok) return 0;

  status = napi_instanceof(env, self, ctor, &bbb);
  if (status != napi_ok) return 0;
  if (!bbb) {
    status = napi_new_instance(env, ctor, argc, argv, &self);
    return status == napi_ok ? self : 0;
  }

  return self;
}

napi_value init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value ctor;

  napi_property_descriptor props[] = {
    { "echo", NULL, echo, NULL, NULL, NULL, napi_enumerable, NULL }
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