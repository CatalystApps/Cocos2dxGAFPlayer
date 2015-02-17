#include "jsb_gaf_manual.h"
#include "cocos2d_specifics.hpp"
#include "GAF_JS.h"
#include "jsb_gaf.hpp"
#include "js_gaf_manual_conversions.h"

extern JSObject* jsb_gaf_GAFObject_prototype;
extern JSObject* jsb_gaf_GAFAsset_prototype;

void register_gaf(JSContext* cx, JSObject* global)
{
    register_all_gaf(cx, global);


    JSObject  *tmpObj = JSVAL_TO_OBJECT(anonEvaluate(cx, global, "(function () { return gaf.Asset; })()"));

    JS_DefineFunction(cx, jsb_gaf_GAFObject_prototype, "setSequenceDelegate", js_gaf_GAFObject_setSequenceDelegate, 1, JSPROP_PERMANENT | JSPROP_ENUMERATE);

    JS_DefineFunction(cx, jsb_gaf_GAFAsset_prototype, "getHeader", js_gaf_GAFAsset_getHeader, 0, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, jsb_gaf_GAFAsset_prototype, "initWithGAFBundle", js_gaf_GAFAsset_initWithGAFBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, jsb_gaf_GAFAsset_prototype, "initWithGAFFile", js_gaf_GAFAsset_initWithGAFFile, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, tmpObj, "create", js_gaf_GAFAsset_create, 2, JSPROP_PERMANENT | JSPROP_ENUMERATE);
    JS_DefineFunction(cx, tmpObj, "createWithBundle", js_gaf_GAFAsset_createWithBundle, 3, JSPROP_PERMANENT | JSPROP_ENUMERATE);

}

bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setSequenceDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *, const std::basic_string<char> &)> arg0;
        do {
            if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
                auto lambda = [=](gaf::GAFObject* larg0, const std::basic_string<char> & larg1) -> void {
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
                    largv[1] = std_string_to_jsval(cx, larg1);
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
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setSequenceDelegate : Error processing arguments");
        cobj->setSequenceDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setSequenceDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}

bool js_gaf_GAFAsset_getHeader(JSContext *cx, uint32_t argc, jsval *vp)
{
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_getHeader : Invalid Native Object");
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

bool js_gaf_GAFAsset_initWithGAFBundle(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_initWithGAFBundle : Invalid Native Object");
    if (argc == 3) {
        std::string arg0;
        std::string arg1;
        std::function<std::basic_string<char>(const std::basic_string<char> &)> arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        do {
            if (JS_TypeOfValue(cx, argv[2]) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[2]));
                auto lambda = [=](const std::basic_string<char> & larg0) -> std::basic_string<char> {
                    jsval largv[1];
                    largv[0] = std_string_to_jsval(cx, larg0);
                    jsval rval;
                    bool ok = func->invoke(1, &largv[0], rval);
                    if (!ok && JS_IsExceptionPending(cx)) {
                        JS_ReportPendingException(cx);
                    }
                    std::basic_string<char> ret;
                    do {
                        if (!jsval_to_std_string(cx, rval, &ret)) 
                        { 
                            ok = false;
                            break;
                        }
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

bool js_gaf_GAFAsset_initWithGAFFile(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFAsset* cobj = (gaf::GAFAsset *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2(cobj, cx, false, "js_gaf_GAFAsset_initWithGAFFile : Invalid Native Object");
    if (argc == 2) {
        std::string arg0;
        std::function<std::basic_string<char>(const std::basic_string<char> &)> arg1;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        do {
            if (JS_TypeOfValue(cx, argv[1]) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[1]));
                auto lambda = [=](const std::basic_string<char> & larg0) -> std::basic_string<char> {
                    jsval largv[1];
                    largv[0] = std_string_to_jsval(cx, larg0);
                    jsval rval;
                    bool ok = func->invoke(1, &largv[0], rval);
                    if (!ok && JS_IsExceptionPending(cx)) {
                        JS_ReportPendingException(cx);
                    }
                    std::basic_string<char> ret;
                    do {
                        if (!jsval_to_std_string(cx, rval, &ret))
                        {
                            ok = false;
                            break;
                        }
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
                }
                else {
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
            std::function<std::basic_string<char>(const std::basic_string<char> &)> arg1;
            do {
                if (JS_TypeOfValue(cx, argv[1]) == JSTYPE_FUNCTION)
                {
                    std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[1]));
                    auto lambda = [=](const std::basic_string<char> & larg0) -> std::basic_string<char> {
                        jsval largv[1];
                        largv[0] = std_string_to_jsval(cx, larg0);
                        jsval rval;
                        bool ok = func->invoke(1, &largv[0], rval);
                        if (!ok && JS_IsExceptionPending(cx)) {
                            JS_ReportPendingException(cx);
                        }
                        std::basic_string<char> ret;
                        do {
                            if (!jsval_to_std_string(cx, rval, &ret))
                            {
                                ok = false;
                                break;
                            }
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
        std::function<std::basic_string<char>(const std::basic_string<char> &)> arg2;
        ok &= jsval_to_std_string(cx, argv[0], &arg0);
        ok &= jsval_to_std_string(cx, argv[1], &arg1);
        do {
            if (JS_TypeOfValue(cx, argv[2]) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[2]));
                auto lambda = [=](const std::basic_string<char> & larg0) -> std::basic_string<char> {
                    jsval largv[1];
                    largv[0] = std_string_to_jsval(cx, larg0);
                    jsval rval;
                    bool ok = func->invoke(1, &largv[0], rval);
                    if (!ok && JS_IsExceptionPending(cx)) {
                        JS_ReportPendingException(cx);
                    }
                    std::basic_string<char> ret;
                    do {
                        if (!jsval_to_std_string(cx, rval, &ret))
                        {
                            ok = false;
                            break;
                        }
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
        JS_SET_RVAL(cx, vp, jsret);
        return true;
    }
    JS_ReportError(cx, "js_gaf_GAFAsset_createWithBundle : wrong number of arguments");
    return false;
}
