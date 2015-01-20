#include "jsapi.h"
#include "jsfriendapi.h"

void register_gaf(JSContext* cx, JSObject* obj);

bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp);
