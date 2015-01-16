
#include "js_gaf_manual_conversions.h"

USING_NS_GAF;

jsval gaf_std_vector_float_to_jsval(JSContext *cx, const std::vector<float>& v)
{
    JSObject *jsretArr = JS_NewArrayObject(cx, 0, NULL);

    int i = 0;
    for (const float obj : v)
    {
        JS::RootedValue arrElement(cx);
        arrElement = DOUBLE_TO_JSVAL(obj);

        if (!JS_SetElement(cx, jsretArr, i, &arrElement)) {
            break;
        }
        ++i;
    }
    return OBJECT_TO_JSVAL(jsretArr);
}

jsval GAFHeader_to_jsval(JSContext *cx, const GAFHeader& header)
{
	JSObject *tmp = JS_NewObject(cx, NULL, NULL, NULL);
	if (!tmp){
		return JSVAL_NULL;
	}
    uint16_t majorVersion = header.getMajorVersion();
    bool ok = JS_DefineProperty(cx, tmp, "compression", UINT_TO_JSVAL(header.compression), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "versionMajor", INT_TO_JSVAL(majorVersion), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "versionMinor", INT_TO_JSVAL(header.getMinorVersion()), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
        JS_DefineProperty(cx, tmp, "fileLength", UINT_TO_JSVAL(header.fileLenght), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT);

	if (!ok)
    {
        return JSVAL_NULL;
    }

    if (majorVersion < 4)
    {
        jsval frameSize = ccrect_to_jsval(cx, header.frameSize);
        jsval pivot = ccpoint_to_jsval(cx, header.pivot);
        ok = JS_DefineProperty(cx, tmp, "framesCount", INT_TO_JSVAL(header.compression), NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "frameSize", frameSize, NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "pivot", pivot, NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    }
    else
    {
        jsval scaleValues = gaf_std_vector_float_to_jsval(cx, header.scaleValues);
        jsval csfValues = gaf_std_vector_float_to_jsval(cx, header.csfValues);
        ok = JS_DefineProperty(cx, tmp, "scaleValues", scaleValues, NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT) &&
            JS_DefineProperty(cx, tmp, "csfValues", csfValues, NULL, NULL, JSPROP_ENUMERATE | JSPROP_PERMANENT);
    }

    if (!ok)
    {
        return JSVAL_NULL;
    }

    return OBJECT_TO_JSVAL(tmp);
}
