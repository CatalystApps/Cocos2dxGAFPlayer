#ifndef __js_gaf_manual_conversions_h__
#define __js_gaf_manual_conversions_h__

#include "ScriptingCore.h"
#include "GAF_JS.h"

USING_NS_GAF;

jsval AnimationMasks_to_jsval(JSContext *cx, const AnimationMasks_t& v);

jsval AnimationObjects_to_jsval(JSContext *cx, const AnimationObjects_t& v);

jsval AnimationFrames_to_jsval(JSContext *cx, const AnimationFrames_t& v);

jsval NamedParts_to_jsval(JSContext *cx, const NamedParts_t& v);

jsval AnimationSequences_to_jsval(JSContext *cx, const AnimationSequences_t& v);

jsval TextsData_to_jsval(JSContext *cx, const TextsData_t& v);

jsval GAFTextureAtlas_to_jsval(JSContext *cx, const GAFTextureAtlas& v);

jsval Timelines_to_jsval(JSContext *cx, const Timelines_t& v);

jsval GAFHeader_to_jsval(JSContext *cx, const GAFHeader& v);

#endif

