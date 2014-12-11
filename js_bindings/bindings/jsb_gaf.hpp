#ifndef __gaf_h__
#define __gaf_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_gaf_GAFAnimationSequence_class;
extern JSObject *jsb_gaf_GAFAnimationSequence_prototype;

bool js_gaf_GAFAnimationSequence_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFAnimationSequence_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFAnimationSequence(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFAnimationSequence_length(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_gaf_GAFAsset_class;
extern JSObject *jsb_gaf_GAFAsset_prototype;

bool js_gaf_GAFAsset_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFAsset_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFAsset(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFAsset_getSceneHeight(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setSceneHeight(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getSceneColor(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setSceneColor(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_createObjectAndRun(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getSceneWidth(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getRootTimeline(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getTimelines(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getSceneFps(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_initWithGAFBundle(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setSceneWidth(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setTextureLoadDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_initWithGAFFile(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setSceneFps(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_createObject(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_getTextureManager(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setRootTimelineWithName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_isAssetVersionPlayable(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_desiredCsf(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_create(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_setDesiredCsf(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_createWithBundle(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAsset_GAFAsset(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_gaf_GAFSprite_class;
extern JSObject *jsb_gaf_GAFSprite_prototype;

bool js_gaf_GAFSprite_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFSprite_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFSprite(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFSprite_setLocator(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFSprite_GAFSprite(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_gaf_GAFObject_class;
extern JSObject *jsb_gaf_GAFObject_prototype;

bool js_gaf_GAFObject_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFObject_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFObject(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFObject_setAnimationStartedNextLoopDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setAnimationFinishedPlayDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setLooped(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getBoundingBoxForCurrentFrame(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setFps(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getObjectByName(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_clearSequence(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getIsAnimationRunning(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_gotoAndStop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getStartFrame(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setFramePlayedDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getCurrentFrameIndex(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getTotalFrameCount(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_start(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_isVisibleInCurrentFrame(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_isDone(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_playSequence(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_stop(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_isReversed(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setFrame(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setControlDelegate(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getEndFrame(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_pauseAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_gotoAndPlay(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_isLooped(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_resumeAnimation(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_setReversed(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_hasSequences(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_getFps(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFObject_GAFObject(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_gaf_GAFAssetTextureManager_class;
extern JSObject *jsb_gaf_GAFAssetTextureManager_prototype;

bool js_gaf_GAFAssetTextureManager_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFAssetTextureManager_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFAssetTextureManager(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFAssetTextureManager_getMemoryConsumptionStat(JSContext *cx, uint32_t argc, jsval *vp);
bool js_gaf_GAFAssetTextureManager_GAFAssetTextureManager(JSContext *cx, uint32_t argc, jsval *vp);

extern JSClass  *jsb_gaf_GAFMovieClip_class;
extern JSObject *jsb_gaf_GAFMovieClip_prototype;

bool js_gaf_GAFMovieClip_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_gaf_GAFMovieClip_finalize(JSContext *cx, JSObject *obj);
void js_register_gaf_GAFMovieClip(JSContext *cx, JSObject *global);
void register_all_gaf(JSContext* cx, JSObject* obj);
bool js_gaf_GAFMovieClip_GAFMovieClip(JSContext *cx, uint32_t argc, jsval *vp);
#endif

