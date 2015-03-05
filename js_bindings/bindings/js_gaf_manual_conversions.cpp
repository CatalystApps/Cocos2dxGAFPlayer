
#include "js_gaf_manual_conversions.h"

USING_NS_GAF;

jsval gaf_std_vector_float_to_jsval(JSContext *cx, const std::vector<float>& v)
{
    JS::RootedObject jsretArr(cx, JS_NewArrayObject(cx, 0));
    
    int i = 0;
    for (const float obj : v)
    {
        if (!JS_SetElement(cx, jsretArr, i, obj)) {
            break;
        }
        ++i;
    }
    return OBJECT_TO_JSVAL(jsretArr);
}

jsval GAFHeader_to_jsval(JSContext *cx, const GAFHeader& header)
{
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
    JS::RootedObject tmp(cx, JS_NewObject(cx, NULL, proto, parent));

    int32_t majorVersion = header.getMajorVersion();
    int32_t minorVersion = header.getMinorVersion();
    bool ok = 
        JS_DefineProperty(cx, tmp, "compression", (uint32_t)header.compression, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "versionMajor", (int32_t)majorVersion, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "versionMinor", (int32_t)minorVersion, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "fileLength", (uint32_t)header.fileLenght, JSPROP_ENUMERATE | JSPROP_PERMANENT);

	if (!ok)
    {
        return JSVAL_NULL;
    }

    if (majorVersion < 4)
    {
        jsval frameSize = ccrect_to_jsval(cx, header.frameSize);
        jsval pivot = ccpoint_to_jsval(cx, header.pivot);

        ok = JS_DefineProperty(cx, tmp, "framesCount", (uint32_t)header.framesCount, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
             JS_DefineProperty(cx, tmp, "frameSize", JS::RootedValue(cx, frameSize), JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
             JS_DefineProperty(cx, tmp, "pivot", JS::RootedValue(cx, pivot), JSPROP_ENUMERATE | JSPROP_PERMANENT);
        if (!ok) return JSVAL_NULL;
    }
    else
    {
        jsval scaleValues = gaf_std_vector_float_to_jsval(cx, header.scaleValues);
        jsval csfValues = gaf_std_vector_float_to_jsval(cx, header.csfValues);
        ok = JS_DefineProperty(cx, tmp, "scaleValues", JS::RootedValue(cx, scaleValues), JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
             JS_DefineProperty(cx, tmp, "csfValues", JS::RootedValue(cx, csfValues), JSPROP_ENUMERATE | JSPROP_PERMANENT);
        if (!ok) return JSVAL_NULL;
    }

    return OBJECT_TO_JSVAL(tmp);
}


jsval GAFAnimationSequence_to_jsval(JSContext *cx, const GAFAnimationSequence& v)
{
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
    JS::RootedObject tmp(cx, JS_NewObject(cx, NULL, proto, parent));

    bool ok = 
        JS_DefineProperty(cx, tmp, "start", (uint32_t)v.startFrameNo, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "end", (uint32_t)v.endFrameNo, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    if (!ok) return JSVAL_NULL;

    return OBJECT_TO_JSVAL(tmp);
}

/*  
unordered_map
< 
    string,
    GAFAnimationSequence
>
*/

jsval GAFAnimationSequences_to_jsval(JSContext *cx, const AnimationSequences_t& v)
{
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
    JS::RootedObject tmp(cx, JS_NewObject(cx, NULL, proto, parent));

    bool ok = true;
    for (auto seq : v)
    {
        jsval seqData = GAFAnimationSequence_to_jsval(cx, seq.second);
        ok = JS_DefineProperty(cx, tmp, seq.second.name.c_str(), JS::RootedValue(cx, seqData), JSPROP_ENUMERATE | JSPROP_PERMANENT);
        if (!ok) return JSVAL_NULL;
    }
    return OBJECT_TO_JSVAL(tmp);
}
