#include "jsapi.h"
#include "jsfriendapi.h"

void register_gaf(JSContext* cx, JS::HandleObject global);

bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_initWithGAFBundle(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_initWithGAFFile(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_create(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_createWithBundle(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getSequences(JSContext *cx, uint32_t argc, jsval *vp);
