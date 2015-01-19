#include "jsapi.h"
#include "jsfriendapi.h"

void register_gaf(JSContext* cx, JSObject* obj);

bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp);
