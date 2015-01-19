#include "jsb_gaf.hpp"
#include "cocos2d_specifics.hpp"
#include "GAF_JS.h"

template<class T>
static bool dummy_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    JS::RootedValue initializing(cx);
    bool isNewValid = true;
    JSObject* global = ScriptingCore::getInstance()->getGlobalObject();
    isNewValid = JS_GetProperty(cx, global, "initializing", &initializing) && JSVAL_TO_BOOLEAN(initializing);
    if (isNewValid)
    {
        TypeTest<T> t;
        js_type_class_t *typeClass = nullptr;
        std::string typeName = t.s_name();
        auto typeMapIter = _js_global_type_map.find(typeName);
        CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
        typeClass = typeMapIter->second;
        CCASSERT(typeClass, "The value is null.");

        JSObject *_tmp = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
        JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(_tmp));
        return true;
    }

    JS_ReportError(cx, "Constructor for the requested class is not available, please refer to the API reference.");
    return false;
}

static bool empty_constructor(JSContext *cx, uint32_t argc, jsval *vp) {
    return false;
}

static bool js_is_native_obj(JSContext *cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
    vp.set(BOOLEAN_TO_JSVAL(true));
    return true;    
}
JSClass  *jsb_gaf_GAFAsset_class;
JSObject *jsb_gaf_GAFAsset_prototype;

bool js_gaf_GAFAsset_getSceneHeight(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_getSceneHeight : Invalid Native Object");
    if (argc == 0) {
        const unsigned int ret = cobj->getSceneHeight();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getSceneHeight : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_setRootTimeline(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    gaf::GAFAsset* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_setRootTimeline : Invalid Native Object");
    do {
        if (argc == 1) {
            unsigned int arg0;
            ok &= jsval_to_uint32(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->setRootTimeline(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->setRootTimeline(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_gaf_GAFAsset_setRootTimeline : wrong number of arguments");
    return false;
}
bool js_gaf_GAFAsset_setSceneHeight(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_setSceneHeight : Invalid Native Object");
    if (argc == 1) {
        unsigned int arg0;
        ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_setSceneHeight : Error processing arguments");
        cobj->setSceneHeight(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_setSceneHeight : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFAsset_getSceneColor(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_getSceneColor : Invalid Native Object");
    if (argc == 0) {
        const cocos2d::Color4B& ret = cobj->getSceneColor();
        jsval jsret = JSVAL_NULL;
        jsret = cccolor4b_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getSceneColor : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_setSceneColor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_setSceneColor : Invalid Native Object");
    if (argc == 1) {
        cocos2d::Color4B arg0;
        ok &= jsval_to_cccolor4b(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_setSceneColor : Error processing arguments");
        cobj->setSceneColor(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_setSceneColor : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFAsset_createObjectAndRun(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_createObjectAndRun : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(JS::RootedValue(cx, argv[0]));
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_createObjectAndRun : Error processing arguments");
        gaf::GAFObject* ret = cobj->createObjectAndRun(arg0);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_createObjectAndRun : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFAsset_getSceneWidth(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_getSceneWidth : Invalid Native Object");
    if (argc == 0) {
        const unsigned int ret = cobj->getSceneWidth();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getSceneWidth : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_getSceneFps(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_getSceneFps : Invalid Native Object");
    if (argc == 0) {
        const unsigned int ret = cobj->getSceneFps();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getSceneFps : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_initWithGAFBundle(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_initWithGAFBundle : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<void (std::basic_string<char> *)> arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        do {
		    if(JS_TypeOfValue(cx, argv[2]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[2]));
		        auto lambda = [=](std::basic_string<char>* larg0) -> void {
		            jsval largv[1];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<std::basic_string<char>>(cx, (std::basic_string<char>*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(1, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg2 = lambda;
		    }
		    else
		    {
		        arg2 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_initWithGAFBundle : Error processing arguments");
        bool ret = cobj->initWithGAFBundle(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_initWithGAFBundle : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}
bool js_gaf_GAFAsset_setSceneWidth(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_setSceneWidth : Invalid Native Object");
    if (argc == 1) {
        unsigned int arg0;
        ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_setSceneWidth : Error processing arguments");
        cobj->setSceneWidth(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_setSceneWidth : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFAsset_initWithGAFFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_initWithGAFFile : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::function<void (std::basic_string<char> *)> arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        do {
		    if(JS_TypeOfValue(cx, argv[1]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[1]));
		        auto lambda = [=](std::basic_string<char>* larg0) -> void {
		            jsval largv[1];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<std::basic_string<char>>(cx, (std::basic_string<char>*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(1, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg1 = lambda;
		    }
		    else
		    {
		        arg1 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_initWithGAFFile : Error processing arguments");
        bool ret = cobj->initWithGAFFile(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_initWithGAFFile : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_gaf_GAFAsset_setSceneFps(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_setSceneFps : Invalid Native Object");
    if (argc == 1) {
        unsigned int arg0;
        ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_setSceneFps : Error processing arguments");
        cobj->setSceneFps(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_setSceneFps : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_getHeader : Invalid Native Object");
    if (argc == 0) {
        const gaf::GAFHeader& ret = cobj->getHeader();
        jsval jsret = JSVAL_NULL;
        jsret = GAFHeader_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getHeader : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_createObject(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFAsset_createObject : Invalid Native Object");
    if (argc == 0) {
        gaf::GAFObject* ret = cobj->createObject();
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_createObject : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFAsset_isAssetVersionPlayable(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 1) {
        const char* arg0;
        std::string arg0_tmp; ok &= jsval_to_std_string(cx, argv[0], &arg0_tmp); arg0 = arg0_tmp.c_str();
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_isAssetVersionPlayable : Error processing arguments");
        bool ret = gaf::GAFAsset::isAssetVersionPlayable(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFAsset_isAssetVersionPlayable : wrong number of arguments");
    return false;
}

bool js_gaf_GAFAsset_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    
    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0);
            jsval jsret = JSVAL_NULL;
            do {
                if (ret) {
                    js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
                    jsret = OBJECT_TO_JSVAL(jsProxy->obj);
                } else {
                    jsret = JSVAL_NULL;
                }
            } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while (0);
    
    do {
        if (argc == 2) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            std::function<void (std::basic_string<char> *)> arg1;
            do {
			    if(JS_TypeOfValue(cx, argv[1]) == JSTYPE_FUNCTION)
			    {
			        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[1]));
			        auto lambda = [=](std::basic_string<char>* larg0) -> void {
			            jsval largv[1];
			            do {
			            if (larg0) {
			                js_proxy_t *jsProxy = js_get_or_create_proxy<std::basic_string<char>>(cx, (std::basic_string<char>*)larg0);
			                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
			            } else {
			                largv[0] = JSVAL_NULL;
			            }
			        } while (0);
			            jsval rval;
			            bool ok = func->invoke(1, &largv[0], rval);
			            if (!ok && JS_IsExceptionPending(cx)) {
			                JS_ReportPendingException(cx);
			            }
			        };
			        arg1 = lambda;
			    }
			    else
			    {
			        arg1 = nullptr;
			    }
			} while(0)
			;
            if (!ok) { ok = true; break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            do {
                if (ret) {
                    js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
                    jsret = OBJECT_TO_JSVAL(jsProxy->obj);
                } else {
                    jsret = JSVAL_NULL;
                }
            } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while (0);
    JS_ReportError(cx, "js_gaf_GAFAsset_create : wrong number of arguments");
    return false;
}
bool js_gaf_GAFAsset_createWithBundle(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<void (std::basic_string<char> *)> arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        do {
		    if(JS_TypeOfValue(cx, argv[2]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[2]));
		        auto lambda = [=](std::basic_string<char>* larg0) -> void {
		            jsval largv[1];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<std::basic_string<char>>(cx, (std::basic_string<char>*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(1, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg2 = lambda;
		    }
		    else
		    {
		        arg2 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_createWithBundle : Error processing arguments");
        gaf::GAFAsset* ret = gaf::GAFAsset::createWithBundle(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFAsset_createWithBundle : wrong number of arguments");
    return false;
}

bool js_gaf_GAFAsset_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    gaf::GAFAsset* cobj = new (std::nothrow) gaf::GAFAsset();
    cocos2d::Ref *_ccobj = dynamic_cast<cocos2d::Ref *>(cobj);
    if (_ccobj) {
        _ccobj->autorelease();
    }
    TypeTest<gaf::GAFAsset> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "gaf::GAFAsset");
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}



void js_gaf_GAFAsset_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (GAFAsset)", obj);
}

void js_register_gaf_GAFAsset(JSContext *cx, JSObject *global) {
    jsb_gaf_GAFAsset_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_gaf_GAFAsset_class->name = "Asset";
    jsb_gaf_GAFAsset_class->addProperty = JS_PropertyStub;
    jsb_gaf_GAFAsset_class->delProperty = JS_DeletePropertyStub;
    jsb_gaf_GAFAsset_class->getProperty = JS_PropertyStub;
    jsb_gaf_GAFAsset_class->setProperty = JS_StrictPropertyStub;
    jsb_gaf_GAFAsset_class->enumerate = JS_EnumerateStub;
    jsb_gaf_GAFAsset_class->resolve = JS_ResolveStub;
    jsb_gaf_GAFAsset_class->convert = JS_ConvertStub;
    jsb_gaf_GAFAsset_class->finalize = js_gaf_GAFAsset_finalize;
    jsb_gaf_GAFAsset_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("getSceneHeight", js_gaf_GAFAsset_getSceneHeight, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setRootTimeline", js_gaf_GAFAsset_setRootTimeline, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setSceneHeight", js_gaf_GAFAsset_setSceneHeight, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getSceneColor", js_gaf_GAFAsset_getSceneColor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setSceneColor", js_gaf_GAFAsset_setSceneColor, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createObjectAndRun", js_gaf_GAFAsset_createObjectAndRun, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getSceneWidth", js_gaf_GAFAsset_getSceneWidth, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getSceneFps", js_gaf_GAFAsset_getSceneFps, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initWithGAFBundle", js_gaf_GAFAsset_initWithGAFBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setSceneWidth", js_gaf_GAFAsset_setSceneWidth, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("initWithGAFFile", js_gaf_GAFAsset_initWithGAFFile, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setSceneFps", js_gaf_GAFAsset_setSceneFps, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getHeader", js_gaf_GAFAsset_getHeader, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createObject", js_gaf_GAFAsset_createObject, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("isAssetVersionPlayable", js_gaf_GAFAsset_isAssetVersionPlayable, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("create", js_gaf_GAFAsset_create, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("createWithBundle", js_gaf_GAFAsset_createWithBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_gaf_GAFAsset_prototype = JS_InitClass(
        cx, global,
        NULL, // parent proto
        jsb_gaf_GAFAsset_class,
        js_gaf_GAFAsset_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "Asset", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<gaf::GAFAsset> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_gaf_GAFAsset_class;
        p->proto = jsb_gaf_GAFAsset_prototype;
        p->parentProto = NULL;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_gaf_GAFSprite_class;
JSObject *jsb_gaf_GAFSprite_prototype;


extern JSObject *jsb_cocos2d_Sprite_prototype;

void js_gaf_GAFSprite_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (GAFSprite)", obj);
}

void js_register_gaf_GAFSprite(JSContext *cx, JSObject *global) {
    jsb_gaf_GAFSprite_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_gaf_GAFSprite_class->name = "Sprite";
    jsb_gaf_GAFSprite_class->addProperty = JS_PropertyStub;
    jsb_gaf_GAFSprite_class->delProperty = JS_DeletePropertyStub;
    jsb_gaf_GAFSprite_class->getProperty = JS_PropertyStub;
    jsb_gaf_GAFSprite_class->setProperty = JS_StrictPropertyStub;
    jsb_gaf_GAFSprite_class->enumerate = JS_EnumerateStub;
    jsb_gaf_GAFSprite_class->resolve = JS_ResolveStub;
    jsb_gaf_GAFSprite_class->convert = JS_ConvertStub;
    jsb_gaf_GAFSprite_class->finalize = js_gaf_GAFSprite_finalize;
    jsb_gaf_GAFSprite_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FS_END
    };

    JSFunctionSpec *st_funcs = NULL;

    jsb_gaf_GAFSprite_prototype = JS_InitClass(
        cx, global,
        jsb_cocos2d_Sprite_prototype,
        jsb_gaf_GAFSprite_class,
        empty_constructor, 0,
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "Sprite", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<gaf::GAFSprite> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_gaf_GAFSprite_class;
        p->proto = jsb_gaf_GAFSprite_prototype;
        p->parentProto = jsb_cocos2d_Sprite_prototype;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

JSClass  *jsb_gaf_GAFObject_class;
JSObject *jsb_gaf_GAFObject_prototype;

bool js_gaf_GAFObject_setAnimationStartedNextLoopDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setAnimationStartedNextLoopDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
		        auto lambda = [=](gaf::GAFObject* larg0) -> void {
		            jsval largv[1];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(1, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setAnimationStartedNextLoopDelegate : Error processing arguments");
        cobj->setAnimationStartedNextLoopDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setAnimationStartedNextLoopDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_setAnimationFinishedPlayDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setAnimationFinishedPlayDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
		        auto lambda = [=](gaf::GAFObject* larg0) -> void {
		            jsval largv[1];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(1, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setAnimationFinishedPlayDelegate : Error processing arguments");
        cobj->setAnimationFinishedPlayDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setAnimationFinishedPlayDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_setLooped(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setLooped : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(JS::RootedValue(cx, argv[0]));
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setLooped : Error processing arguments");
        cobj->setLooped(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setLooped : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_getBoundingBoxForCurrentFrame(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getBoundingBoxForCurrentFrame : Invalid Native Object");
    if (argc == 0) {
        cocos2d::Rect ret = cobj->getBoundingBoxForCurrentFrame();
        jsval jsret = JSVAL_NULL;
        jsret = ccrect_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getBoundingBoxForCurrentFrame : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_setFps(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setFps : Invalid Native Object");
    if (argc == 1) {
        unsigned int arg0;
        ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setFps : Error processing arguments");
        cobj->setFps(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setFps : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_getObjectByName(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    gaf::GAFObject* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getObjectByName : Invalid Native Object");
    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            const gaf::GAFObject* ret = cobj->getObjectByName(arg0);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            gaf::GAFObject* ret = cobj->getObjectByName(arg0);
            jsval jsret = JSVAL_NULL;
            do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            } else {
                jsret = JSVAL_NULL;
            }
        } while (0);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_gaf_GAFObject_getObjectByName : wrong number of arguments");
    return false;
}
bool js_gaf_GAFObject_clearSequence(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_clearSequence : Invalid Native Object");
    if (argc == 0) {
        cobj->clearSequence();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_clearSequence : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_getIsAnimationRunning(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getIsAnimationRunning : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->getIsAnimationRunning();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getIsAnimationRunning : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_gotoAndStop(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    gaf::GAFObject* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_gotoAndStop : Invalid Native Object");
    do {
        if (argc == 1) {
            unsigned int arg0;
            ok &= jsval_to_uint32(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->gotoAndStop(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->gotoAndStop(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_gaf_GAFObject_gotoAndStop : wrong number of arguments");
    return false;
}
bool js_gaf_GAFObject_getStartFrame(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getStartFrame : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_getStartFrame : Error processing arguments");
        unsigned int ret = cobj->getStartFrame(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getStartFrame : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_setFramePlayedDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setFramePlayedDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *, unsigned int)> arg0;
        do {
		    if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
		        auto lambda = [=](gaf::GAFObject* larg0, unsigned int larg1) -> void {
		            jsval largv[2];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            largv[1] = uint32_to_jsval(cx, larg1);
		            jsval rval;
		            bool ok = func->invoke(2, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setFramePlayedDelegate : Error processing arguments");
        cobj->setFramePlayedDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setFramePlayedDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_getCurrentFrameIndex(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getCurrentFrameIndex : Invalid Native Object");
    if (argc == 0) {
        unsigned int ret = cobj->getCurrentFrameIndex();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getCurrentFrameIndex : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_getTotalFrameCount(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getTotalFrameCount : Invalid Native Object");
    if (argc == 0) {
        unsigned int ret = cobj->getTotalFrameCount();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getTotalFrameCount : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_start(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_start : Invalid Native Object");
    if (argc == 0) {
        cobj->start();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_start : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_init(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_init : Invalid Native Object");
    if (argc == 2) {
        gaf::GAFAsset* arg0;
        gaf::GAFTimeline* arg1;
        do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (gaf::GAFAsset*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (!argv[1].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[1]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (gaf::GAFTimeline*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_init : Error processing arguments");
        bool ret = cobj->init(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_init : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;
}
bool js_gaf_GAFObject_isDone(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_isDone : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isDone();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_isDone : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_playSequence(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_playSequence : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_playSequence : Error processing arguments");
        bool ret = cobj->playSequence(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 2) {
        std::string arg0;
        bool arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        arg1 = JS::ToBoolean(JS::RootedValue(cx, argv[1]));
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_playSequence : Error processing arguments");
        bool ret = cobj->playSequence(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    if (argc == 3) {
        std::string arg0;
        bool arg1;
        bool arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        arg1 = JS::ToBoolean(JS::RootedValue(cx, argv[1]));
        arg2 = JS::ToBoolean(JS::RootedValue(cx, argv[2]));
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_playSequence : Error processing arguments");
        bool ret = cobj->playSequence(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_playSequence : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_stop(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_stop : Invalid Native Object");
    if (argc == 0) {
        cobj->stop();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_stop : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_isReversed(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_isReversed : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isReversed();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_isReversed : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_setFrame(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setFrame : Invalid Native Object");
    if (argc == 1) {
        unsigned int arg0;
        ok &= jsval_to_uint32(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setFrame : Error processing arguments");
        bool ret = cobj->setFrame(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setFrame : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_setControlDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setControlDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *, const gaf::GAFSprite *)> arg0;
        do {
		    if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
		    {
		        std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
		        auto lambda = [=](gaf::GAFObject* larg0, const gaf::GAFSprite* larg1) -> void {
		            jsval largv[2];
		            do {
		            if (larg0) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
		                largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[0] = JSVAL_NULL;
		            }
		        } while (0);
		            do {
		            if (larg1) {
		                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFSprite>(cx, (gaf::GAFSprite*)larg1);
		                largv[1] = OBJECT_TO_JSVAL(jsProxy->obj);
		            } else {
		                largv[1] = JSVAL_NULL;
		            }
		        } while (0);
		            jsval rval;
		            bool ok = func->invoke(2, &largv[0], rval);
		            if (!ok && JS_IsExceptionPending(cx)) {
		                JS_ReportPendingException(cx);
		            }
		        };
		        arg0 = lambda;
		    }
		    else
		    {
		        arg0 = nullptr;
		    }
		} while(0)
		;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setControlDelegate : Error processing arguments");
        cobj->setControlDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setControlDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_getEndFrame(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getEndFrame : Invalid Native Object");
    if (argc == 1) {
        std::string arg0;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_getEndFrame : Error processing arguments");
        unsigned int ret = cobj->getEndFrame(arg0);
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getEndFrame : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_pauseAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_pauseAnimation : Invalid Native Object");
    if (argc == 0) {
        cobj->pauseAnimation();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_pauseAnimation : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_gotoAndPlay(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;

    JSObject *obj = NULL;
    gaf::GAFObject* cobj = NULL;
    obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_gotoAndPlay : Invalid Native Object");
    do {
        if (argc == 1) {
            unsigned int arg0;
            ok &= jsval_to_uint32(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->gotoAndPlay(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, argv[0], &arg0);
            if (!ok) { ok = true; break; }
            bool ret = cobj->gotoAndPlay(arg0);
            jsval jsret = JSVAL_NULL;
            jsret = BOOLEAN_TO_JSVAL(ret);
            JS_SET_RVAL(cx, vp, jsret);
            return true;
        }
    } while(0);

    JS_ReportError(cx, "js_gaf_GAFObject_gotoAndPlay : wrong number of arguments");
    return false;
}
bool js_gaf_GAFObject_isLooped(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_isLooped : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->isLooped();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_isLooped : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_resumeAnimation(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_resumeAnimation : Invalid Native Object");
    if (argc == 0) {
        cobj->resumeAnimation();
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_resumeAnimation : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_setReversed(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setReversed : Invalid Native Object");
    if (argc == 1) {
        bool arg0;
        arg0 = JS::ToBoolean(JS::RootedValue(cx, argv[0]));
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setReversed : Error processing arguments");
        cobj->setReversed(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setReversed : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
bool js_gaf_GAFObject_hasSequences(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_hasSequences : Invalid Native Object");
    if (argc == 0) {
        bool ret = cobj->hasSequences();
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_hasSequences : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_getFps(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_getFps : Invalid Native Object");
    if (argc == 0) {
        unsigned int ret = cobj->getFps();
        jsval jsret = JSVAL_NULL;
        jsret = uint32_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_getFps : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
bool js_gaf_GAFObject_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    if (argc == 2) {
        gaf::GAFAsset* arg0;
        gaf::GAFTimeline* arg1;
        do {
            if (!argv[0].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[0]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg0 = (gaf::GAFAsset*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg0, cx, false, "Invalid Native Object");
        } while (0);
        do {
            if (!argv[1].isObject()) { ok = false; break; }
            js_proxy_t *jsProxy;
            JSObject *tmpObj = JSVAL_TO_OBJECT(argv[1]);
            jsProxy = jsb_get_js_proxy(tmpObj);
            arg1 = (gaf::GAFTimeline*)(jsProxy ? jsProxy->ptr : NULL);
            JSB_PRECONDITION2( arg1, cx, false, "Invalid Native Object");
        } while (0);
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_create : Error processing arguments");
        gaf::GAFObject* ret = gaf::GAFObject::create(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        do {
        if (ret) {
            js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)ret);
            jsret = OBJECT_TO_JSVAL(jsProxy->obj);
        } else {
            jsret = JSVAL_NULL;
        }
    } while (0);
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFObject_create : wrong number of arguments");
    return false;
}

bool js_gaf_GAFObject_constructor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    gaf::GAFObject* cobj = new (std::nothrow) gaf::GAFObject();
    cocos2d::Ref *_ccobj = dynamic_cast<cocos2d::Ref *>(cobj);
    if (_ccobj) {
        _ccobj->autorelease();
    }
    TypeTest<gaf::GAFObject> t;
    js_type_class_t *typeClass = nullptr;
    std::string typeName = t.s_name();
    auto typeMapIter = _js_global_type_map.find(typeName);
    CCASSERT(typeMapIter != _js_global_type_map.end(), "Can't find the class type!");
    typeClass = typeMapIter->second;
    CCASSERT(typeClass, "The value is null.");
    JSObject *obj = JS_NewObject(cx, typeClass->jsclass, typeClass->proto, typeClass->parentProto);
    JS_SET_RVAL(cx, vp, OBJECT_TO_JSVAL(obj));
    // link the native object with the javascript object
    js_proxy_t* p = jsb_new_proxy(cobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "gaf::GAFObject");
    if (JS_HasProperty(cx, obj, "_ctor", &ok) && ok)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    return true;
}


extern JSObject *jsb_gaf_GAFSprite_prototype;

void js_gaf_GAFObject_finalize(JSFreeOp *fop, JSObject *obj) {
    CCLOGINFO("jsbindings: finalizing JS object %p (GAFObject)", obj);
}

static bool js_gaf_GAFObject_ctor(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    gaf::GAFObject *nobj = new (std::nothrow) gaf::GAFObject();
    if (nobj) {
        nobj->autorelease();
    }
    js_proxy_t* p = jsb_new_proxy(nobj, obj);
    JS_AddNamedObjectRoot(cx, &p->obj, "gaf::GAFObject");
    bool isFound = false;
    if (JS_HasProperty(cx, obj, "_ctor", &isFound) && isFound)
        ScriptingCore::getInstance()->executeFunctionWithOwner(OBJECT_TO_JSVAL(obj), "_ctor", argc, argv);
    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return true;
}
void js_register_gaf_GAFObject(JSContext *cx, JSObject *global) {
    jsb_gaf_GAFObject_class = (JSClass *)calloc(1, sizeof(JSClass));
    jsb_gaf_GAFObject_class->name = "Object";
    jsb_gaf_GAFObject_class->addProperty = JS_PropertyStub;
    jsb_gaf_GAFObject_class->delProperty = JS_DeletePropertyStub;
    jsb_gaf_GAFObject_class->getProperty = JS_PropertyStub;
    jsb_gaf_GAFObject_class->setProperty = JS_StrictPropertyStub;
    jsb_gaf_GAFObject_class->enumerate = JS_EnumerateStub;
    jsb_gaf_GAFObject_class->resolve = JS_ResolveStub;
    jsb_gaf_GAFObject_class->convert = JS_ConvertStub;
    jsb_gaf_GAFObject_class->finalize = js_gaf_GAFObject_finalize;
    jsb_gaf_GAFObject_class->flags = JSCLASS_HAS_RESERVED_SLOTS(2);

    static JSPropertySpec properties[] = {
        {"__nativeObj", 0, JSPROP_ENUMERATE | JSPROP_PERMANENT, JSOP_WRAPPER(js_is_native_obj), JSOP_NULLWRAPPER},
        {0, 0, 0, JSOP_NULLWRAPPER, JSOP_NULLWRAPPER}
    };

    static JSFunctionSpec funcs[] = {
        JS_FN("setAnimationStartedNextLoopDelegate", js_gaf_GAFObject_setAnimationStartedNextLoopDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setAnimationFinishedPlayDelegate", js_gaf_GAFObject_setAnimationFinishedPlayDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setLooped", js_gaf_GAFObject_setLooped, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getBoundingBoxForCurrentFrame", js_gaf_GAFObject_getBoundingBoxForCurrentFrame, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setFps", js_gaf_GAFObject_setFps, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getObjectByName", js_gaf_GAFObject_getObjectByName, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("clearSequence", js_gaf_GAFObject_clearSequence, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getIsAnimationRunning", js_gaf_GAFObject_getIsAnimationRunning, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoAndStop", js_gaf_GAFObject_gotoAndStop, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getStartFrame", js_gaf_GAFObject_getStartFrame, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setFramePlayedDelegate", js_gaf_GAFObject_setFramePlayedDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getCurrentFrameIndex", js_gaf_GAFObject_getCurrentFrameIndex, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getTotalFrameCount", js_gaf_GAFObject_getTotalFrameCount, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("start", js_gaf_GAFObject_start, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("init", js_gaf_GAFObject_init, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isDone", js_gaf_GAFObject_isDone, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("playSequence", js_gaf_GAFObject_playSequence, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("stop", js_gaf_GAFObject_stop, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isReversed", js_gaf_GAFObject_isReversed, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setFrame", js_gaf_GAFObject_setFrame, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setControlDelegate", js_gaf_GAFObject_setControlDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getEndFrame", js_gaf_GAFObject_getEndFrame, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("pauseAnimation", js_gaf_GAFObject_pauseAnimation, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("gotoAndPlay", js_gaf_GAFObject_gotoAndPlay, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("isLooped", js_gaf_GAFObject_isLooped, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("resumeAnimation", js_gaf_GAFObject_resumeAnimation, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("setReversed", js_gaf_GAFObject_setReversed, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("hasSequences", js_gaf_GAFObject_hasSequences, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("getFps", js_gaf_GAFObject_getFps, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FN("ctor", js_gaf_GAFObject_ctor, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    static JSFunctionSpec st_funcs[] = {
        JS_FN("create", js_gaf_GAFObject_create, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE),
        JS_FS_END
    };

    jsb_gaf_GAFObject_prototype = JS_InitClass(
        cx, global,
        jsb_gaf_GAFSprite_prototype,
        jsb_gaf_GAFObject_class,
        js_gaf_GAFObject_constructor, 0, // constructor
        properties,
        funcs,
        NULL, // no static properties
        st_funcs);
    // make the class enumerable in the registered namespace
//  bool found;
//FIXME: Removed in Firefox v27 
//  JS_SetPropertyAttributes(cx, global, "Object", JSPROP_ENUMERATE | JSPROP_READONLY, &found);

    // add the proto and JSClass to the type->js info hash table
    TypeTest<gaf::GAFObject> t;
    js_type_class_t *p;
    std::string typeName = t.s_name();
    if (_js_global_type_map.find(typeName) == _js_global_type_map.end())
    {
        p = (js_type_class_t *)malloc(sizeof(js_type_class_t));
        p->jsclass = jsb_gaf_GAFObject_class;
        p->proto = jsb_gaf_GAFObject_prototype;
        p->parentProto = jsb_gaf_GAFSprite_prototype;
        _js_global_type_map.insert(std::make_pair(typeName, p));
    }
}

void register_all_gaf(JSContext* cx, JSObject* obj) {
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject ns(cx);
    JS_GetProperty(cx, obj, "gaf", &nsval);
    if (nsval == JSVAL_VOID) {
        ns = JS_NewObject(cx, NULL, NULL, NULL);
        nsval = OBJECT_TO_JSVAL(ns);
        JS_SetProperty(cx, obj, "gaf", nsval);
    } else {
        JS_ValueToObject(cx, nsval, &ns);
    }
    obj = ns;

    js_register_gaf_GAFAsset(cx, obj);
    js_register_gaf_GAFSprite(cx, obj);
    js_register_gaf_GAFObject(cx, obj);
}

