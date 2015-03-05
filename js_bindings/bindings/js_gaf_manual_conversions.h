#ifndef __js_gaf_manual_conversions_h__
#define __js_gaf_manual_conversions_h__

#include "ScriptingCore.h"
#include "GAF_JS.h"

USING_NS_GAF;

jsval GAFHeader_to_jsval(JSContext *cx, const GAFHeader& v);
jsval GAFAnimationSequence_to_jsval(JSContext *cx, const GAFAnimationSequence& v);
jsval GAFAnimationSequences_to_jsval(JSContext *cx, const AnimationSequences_t& v);

#endif

