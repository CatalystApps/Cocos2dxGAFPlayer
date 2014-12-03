
#include "js_gaf_manual_conversions.h"

USING_NS_GAF;

jsval AnimationMasks_to_jsval(JSContext *cx, const AnimationMasks_t& v)
{/*
 JSObject *tmp = JS_NewObject(cx, NULL, NULL, NULL);
 if (!tmp) return JSVAL_NULL;
 bool ok = JS_DefineProperty(cx, tmp, "x", DOUBLE_TO_JSVAL(v.x), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
 JS_DefineProperty(cx, tmp, "y", DOUBLE_TO_JSVAL(v.y), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT);
 if (ok) {
 return OBJECT_TO_JSVAL(tmp);
 }*/
    return JSVAL_NULL;
}

jsval AnimationObjects_to_jsval(JSContext *cx, const AnimationObjects_t& v)
{
    return JSVAL_NULL;
}

jsval AnimationFrames_to_jsval(JSContext *cx, const AnimationFrames_t& v)
{
    return JSVAL_NULL;
}

jsval NamedParts_to_jsval(JSContext *cx, const NamedParts_t& v)
{
    return JSVAL_NULL;
}

jsval AnimationSequences_to_jsval(JSContext *cx, const AnimationSequences_t& v)
{
    return JSVAL_NULL;
}

jsval TextsData_to_jsval(JSContext *cx, const TextsData_t& v)
{
    return JSVAL_NULL;
}

jsval GAFTextureAtlas_to_jsval(JSContext *cx, const GAFTextureAtlas& v)
{
    return JSVAL_NULL;
}

jsval Timelines_to_jsval(JSContext *cx, const Timelines_t& v)
{
    return JSVAL_NULL;
}

jsval GAFHeader_to_jsval(JSContext *cx, const GAFHeader& v)
{
    return JSVAL_NULL;
}
