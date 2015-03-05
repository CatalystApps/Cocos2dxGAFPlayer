#include "jsb_gaf_manual.h"
#include "cocos2d_specifics.hpp"
#include "GAF_JS.h"
#include "jsb_gaf.hpp"
#include "js_gaf_manual_conversions.h"

extern JSObject* jsb_gaf_GAFObject_prototype;
extern JSObject* jsb_gaf_GAFAsset_prototype;

void register_gaf(JSContext* cx, JS::HandleObject global)
{
    register_all_gaf(cx, global);

    JS::RootedObject gafObjectProto(cx, jsb_gaf_GAFObject_prototype);
    JS::RootedObject gafAssetProto(cx, jsb_gaf_GAFAsset_prototype);
    
    JS::RootedObject gafAsset(cx);
    gafAsset = anonEvaluate(cx, global, "(function () { return gaf.Asset; })()").toObjectOrNull();;

    // methods of GAFObject
    JS_DefineFunction(cx, gafObjectProto, "setSequenceDelegate", js_gaf_GAFObject_setSequenceDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, gafObjectProto, "getSequences", js_gaf_GAFObject_getSequences, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE);

    // methods of GAFAsset
    JS_DefineFunction(cx, gafAssetProto, "getHeader", js_gaf_GAFAsset_getHeader, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, gafAssetProto, "initWithGAFBundle", js_gaf_GAFAsset_initWithGAFBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, gafAssetProto, "initWithGAFFile", js_gaf_GAFAsset_initWithGAFFile, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE);

    // static methods of GAFAsset
    JS_DefineFunction(cx, gafAsset, "create", js_gaf_GAFAsset_create, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, gafAsset, "createWithBundle", js_gaf_GAFAsset_createWithBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE);

}

bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(obj, cx, false, "js_gaf_GAFObject_setSequenceDelegate : Invalid Native Object");
    if (argc == 1) {
        GAFSequenceDelegate_t delegate = nullptr;
        JS::HandleValue arg0 = args.get(0);
        if (JS_TypeOfValue(cx, arg0) == JSTYPE_FUNCTION)
        {
            std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, obj, arg0));
            delegate = [=](GAFObject* larg0, const std::string& larg1) -> void
            {
                JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                jsval largv[2];
                    largv[0] = JSVAL_NULL;
                if (larg0) 
                {
                    js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
                    largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
                }
                largv[1] = std_string_to_jsval(cx, larg1);
                JS::RootedValue rval(cx);
                bool ok = func->invoke(1, &largv[0], &rval);
                if (!ok && JS_IsExceptionPending(cx)) {
                    JS_ReportPendingException(cx);
                }
            };
        }
        cobj->setSequenceDelegate(delegate);
        args.rval().set(JSVAL_NULL);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setSequenceDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}

bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_getHeader : Invalid Native Object");
    if (argc == 0) {
        const gaf::GAFHeader& ret = cobj->getHeader();
        jsval jsret = JSVAL_NULL;
        args.rval().set(GAFHeader_to_jsval(cx, ret));
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_getHeader : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}

bool js_gaf_GAFAsset_initWithGAFBundle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_initWithGAFBundle : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        GAFTextureLoadDelegate_t arg2 = nullptr;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);

        if (JS_TypeOfValue(cx, args.get(2)) == JSTYPE_FUNCTION)
        {
            std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, args.thisv().toObjectOrNull(), args.get(2)));
            auto lambda = [=](const std::string& larg0) -> std::string {
                JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                jsval largv[1];
                largv[0] = std_string_to_jsval(cx, larg0);
                JS::RootedValue rval(cx);
                bool ok = func->invoke(1, &largv[0], &rval);
                if (!ok && JS_IsExceptionPending(cx)) {
                    JS_ReportPendingException(cx);
                }
                std::string ret;
                do {
                    if (!rval.isObject()) { ok = false; break; }
                    js_proxy_t *jsProxy;
                    JSObject *tmpObj = rval.toObjectOrNull();
                    jsval_to_std_string(cx, rval, &ret);
                } while (0);
                return ret;
            };
            arg2 = lambda;
        }

        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_initWithGAFBundle : Error processing arguments");
        bool ret = cobj->initWithGAFBundle(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_initWithGAFBundle : wrong number of arguments: %d, was expecting %d", argc, 3);
    return false;
}

bool js_gaf_GAFAsset_initWithGAFFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_initWithGAFFile : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::function<std::basic_string<char>(const std::basic_string<char> &)> arg1;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        if (JS_TypeOfValue(cx, args.get(2)) == JSTYPE_FUNCTION)
        {
            std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, args.thisv().toObjectOrNull(), args.get(1)));
            auto lambda = [=](const std::string& larg0) -> std::string {
                JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                jsval largv[1];
                largv[0] = std_string_to_jsval(cx, larg0);
                JS::RootedValue rval(cx);
                bool ok = func->invoke(1, &largv[0], &rval);
                if (!ok && JS_IsExceptionPending(cx)) {
                    JS_ReportPendingException(cx);
                }
                std::string ret;
                do {
                    if (!rval.isObject()) { ok = false; break; }
                    js_proxy_t *jsProxy;
                    JSObject *tmpObj = rval.toObjectOrNull();
                    jsval_to_std_string(cx, rval, &ret);
                } while (0);
                return ret;
            };
            arg1 = lambda;
        }
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_initWithGAFFile : Error processing arguments");
        bool ret = cobj->initWithGAFFile(arg0, arg1);
        jsval jsret = JSVAL_NULL;
        jsret = BOOLEAN_TO_JSVAL(ret);
        args.rval().set(jsret);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFAsset_initWithGAFFile : wrong number of arguments: %d, was expecting %d", argc, 2);
    return false;

}

bool js_gaf_GAFAsset_create(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
        if (argc == 1) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0);
            jsval jsret = JSVAL_NULL;
            do {
                if (ret) {
                    js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
                    jsret = OBJECT_TO_JSVAL(jsProxy->obj);
                }
                else {
                    jsret = JSVAL_NULL;
                }
            } while (0);
            args.rval().set(jsret);
            return true;
        }
    } while (0);

    do {
        if (argc == 2) {
            std::string arg0;
            ok &= jsval_to_std_string(cx, args.get(0), &arg0);
            if (!ok) { ok = true; break; }
            std::function<std::basic_string<char>(const std::basic_string<char> &)> arg1;
            do {
                if (JS_TypeOfValue(cx, args.get(1)) == JSTYPE_FUNCTION)
                {
                    std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, args.thisv().toObjectOrNull(), args.get(1)));
                    auto lambda = [=](const std::string& larg0) -> std::string {
                        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                            jsval largv[1];
                        largv[0] = std_string_to_jsval(cx, larg0);
                        JS::RootedValue rval(cx);
                        bool ok = func->invoke(1, &largv[0], &rval);
                        if (!ok && JS_IsExceptionPending(cx)) {
                            JS_ReportPendingException(cx);
                        }
                        std::string ret;
                        do {
                            if (!rval.isObject()) { ok = false; break; }
                            js_proxy_t *jsProxy;
                            JSObject *tmpObj = rval.toObjectOrNull();
                            jsval_to_std_string(cx, rval, &ret);
                        } while (0);
                        return ret;
                    };
                    arg1 = lambda;
                }
                else
                {
                    arg1 = nullptr;
                }
            } while (0)
                ;
            if (!ok) { ok = true; break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0, arg1);
            jsval jsret = JSVAL_NULL;
            do {
                if (ret) {
                    js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
                    jsret = OBJECT_TO_JSVAL(jsProxy->obj);
                }
                else {
                    jsret = JSVAL_NULL;
                }
            } while (0);
            args.rval().set(jsret);
            return true;
        }
    } while (0);
    JS_ReportError(cx, "js_gaf_GAFAsset_create : wrong number of arguments");
    return false;
}

bool js_gaf_GAFAsset_createWithBundle(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<std::basic_string<char>(const std::basic_string<char> &)> arg2;
        ok &= jsval_to_std_string(cx, args.get(0), &arg0);
        ok &= jsval_to_std_string(cx, args.get(1), &arg1);
        do {
            if (JS_TypeOfValue(cx, args.get(2)) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, args.thisv().toObjectOrNull(), args.get(2)));
                auto lambda = [=](const std::string& larg0) -> std::string {
                    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
                        jsval largv[1];
                    largv[0] = std_string_to_jsval(cx, larg0);
                    JS::RootedValue rval(cx);
                    bool ok = func->invoke(1, &largv[0], &rval);
                    if (!ok && JS_IsExceptionPending(cx)) {
                        JS_ReportPendingException(cx);
                    }
                    std::string ret;
                    do {
                        if (!rval.isObject()) { ok = false; break; }
                        js_proxy_t *jsProxy;
                        JSObject *tmpObj = rval.toObjectOrNull();
                        jsval_to_std_string(cx, rval, &ret);
                    } while (0);
                    return ret;
                };
                arg2 = lambda;
            }
            else
            {
                arg2 = nullptr;
            }
        } while (0)
            ;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFAsset_createWithBundle : Error processing arguments");
        gaf::GAFAsset* ret = gaf::GAFAsset::createWithBundle(arg0, arg1, arg2);
        jsval jsret = JSVAL_NULL;
        do {
            if (ret) {
                js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFAsset>(cx, (gaf::GAFAsset*)ret);
                jsret = OBJECT_TO_JSVAL(jsProxy->obj);
            }
            else {
                jsret = JSVAL_NULL;
            }
        } while (0);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFAsset_createWithBundle : wrong number of arguments");
    return false;

}

bool js_gaf_GAFObject_getSequences(JSContext *cx, uint32_t argc, jsval *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    JS::RootedObject obj(cx, args.thisv().toObjectOrNull());
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFObject_getSequences : Invalid Native Object");
    if (argc == 0) {
        const AnimationSequences_t& ret = cobj->getSequences();
        jsval jsret = GAFAnimationSequences_to_jsval(cx, ret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFObject_getSequences : wrong number of arguments: %d, was expecting %d", argc, 0);
    return false;
}
