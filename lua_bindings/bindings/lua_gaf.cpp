#include "lua_gaf.hpp"
#include "GAF_LUA.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_gaf_GAFAnimationSequence_length(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAnimationSequence* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAnimationSequence",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAnimationSequence*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAnimationSequence_length'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAnimationSequence_length'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->length();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAnimationSequence:length",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAnimationSequence_length'.",&tolua_err);
#endif

    return 0;
}
static int lua_gaf_GAFAnimationSequence_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFAnimationSequence)");
    return 0;
}

int lua_register_gaf_GAFAnimationSequence(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFAnimationSequence");
    tolua_cclass(tolua_S,"GAFAnimationSequence","gaf.GAFAnimationSequence","",nullptr);

    tolua_beginmodule(tolua_S,"GAFAnimationSequence");
        tolua_function(tolua_S,"length",lua_gaf_GAFAnimationSequence_length);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFAnimationSequence).name();
    g_luaType[typeName] = "gaf.GAFAnimationSequence";
    g_typeCast["GAFAnimationSequence"] = "gaf.GAFAnimationSequence";
    return 1;
}

int lua_gaf_GAFAsset_getSceneHeight(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getSceneHeight'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getSceneHeight'", nullptr);
            return 0;
        }
        const unsigned int ret = cobj->getSceneHeight();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getSceneHeight",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getSceneHeight'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setSceneHeight(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setSceneHeight'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setSceneHeight'", nullptr);
            return 0;
        }
        cobj->setSceneHeight(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setSceneHeight",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setSceneHeight'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getSceneColor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getSceneColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getSceneColor'", nullptr);
            return 0;
        }
        const cocos2d::Color4B& ret = cobj->getSceneColor();
        color4b_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getSceneColor",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getSceneColor'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setSceneColor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setSceneColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Color4B arg0;

        ok &=luaval_to_color4b(tolua_S, 2, &arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setSceneColor'", nullptr);
            return 0;
        }
        cobj->setSceneColor(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setSceneColor",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setSceneColor'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getGAFFileName(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getGAFFileName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getGAFFileName'", nullptr);
            return 0;
        }
        const std::string& ret = cobj->getGAFFileName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getGAFFileName",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getGAFFileName'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_createObjectAndRun(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_createObjectAndRun'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_createObjectAndRun'", nullptr);
            return 0;
        }
        gaf::GAFObject* ret = cobj->createObjectAndRun(arg0);
        object_to_luaval<gaf::GAFObject>(tolua_S, "gaf.GAFObject",(gaf::GAFObject*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:createObjectAndRun",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_createObjectAndRun'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setDesiredAtlasScale(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setDesiredAtlasScale'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setDesiredAtlasScale'", nullptr);
            return 0;
        }
        cobj->setDesiredAtlasScale(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setDesiredAtlasScale",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setDesiredAtlasScale'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getSceneWidth(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getSceneWidth'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getSceneWidth'", nullptr);
            return 0;
        }
        const unsigned int ret = cobj->getSceneWidth();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getSceneWidth",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getSceneWidth'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getRootTimeline(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getRootTimeline'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getRootTimeline'", nullptr);
            return 0;
        }
        gaf::GAFTimeline* ret = cobj->getRootTimeline();
        object_to_luaval<gaf::GAFTimeline>(tolua_S, "gaf.GAFTimeline",(gaf::GAFTimeline*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getRootTimeline",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getRootTimeline'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getSceneFps(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getSceneFps'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getSceneFps'", nullptr);
            return 0;
        }
        const unsigned int ret = cobj->getSceneFps();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getSceneFps",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getSceneFps'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_desiredAtlasScale(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_desiredAtlasScale'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_desiredAtlasScale'", nullptr);
            return 0;
        }
        double ret = cobj->desiredAtlasScale();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:desiredAtlasScale",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_desiredAtlasScale'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_initWithGAFBundle(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_initWithGAFBundle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        std::string arg1;
        std::function<std::basic_string<char> (const std::basic_string<char> &)> arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_initWithGAFBundle'", nullptr);
            return 0;
        }
        bool ret = cobj->initWithGAFBundle(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:initWithGAFBundle",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_initWithGAFBundle'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setSceneWidth(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setSceneWidth'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setSceneWidth'", nullptr);
            return 0;
        }
        cobj->setSceneWidth(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setSceneWidth",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setSceneWidth'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setTextureLoadDelegate(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setTextureLoadDelegate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<std::basic_string<char> (const std::basic_string<char> &)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setTextureLoadDelegate'", nullptr);
            return 0;
        }
        cobj->setTextureLoadDelegate(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setTextureLoadDelegate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setTextureLoadDelegate'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_initWithGAFFile(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_initWithGAFFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::function<std::basic_string<char> (const std::basic_string<char> &)> arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_initWithGAFFile'", nullptr);
            return 0;
        }
        bool ret = cobj->initWithGAFFile(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:initWithGAFFile",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_initWithGAFFile'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_setSceneFps(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_setSceneFps'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_setSceneFps'", nullptr);
            return 0;
        }
        cobj->setSceneFps(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:setSceneFps",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_setSceneFps'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_createObject(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_createObject'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_createObject'", nullptr);
            return 0;
        }
        gaf::GAFObject* ret = cobj->createObject();
        object_to_luaval<gaf::GAFObject>(tolua_S, "gaf.GAFObject",(gaf::GAFObject*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:createObject",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_createObject'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getTextureManager(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getTextureManager'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getTextureManager'", nullptr);
            return 0;
        }
        gaf::GAFAssetTextureManager* ret = cobj->getTextureManager();
        object_to_luaval<gaf::GAFAssetTextureManager>(tolua_S, "gaf.GAFAssetTextureManager",(gaf::GAFAssetTextureManager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getTextureManager",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getTextureManager'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_getTimelineByName(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAsset*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAsset_getTimelineByName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_getTimelineByName'", nullptr);
            return 0;
        }
        gaf::GAFTimeline* ret = cobj->getTimelineByName(arg0);
        object_to_luaval<gaf::GAFTimeline>(tolua_S, "gaf.GAFTimeline",(gaf::GAFTimeline*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:getTimelineByName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_getTimelineByName'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAsset_isAssetVersionPlayable(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_isAssetVersionPlayable'", nullptr);
            return 0;
        }
        bool ret = gaf::GAFAsset::isAssetVersionPlayable(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "gaf.GAFAsset:isAssetVersionPlayable",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_isAssetVersionPlayable'.",&tolua_err);
#endif
    return 0;
}
int lua_gaf_GAFAsset_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 1)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);
            if (!ok) { break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0);
            object_to_luaval<gaf::GAFAsset>(tolua_S, "gaf.GAFAsset",(gaf::GAFAsset*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 2)
        {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);
            if (!ok) { break; }
            std::function<std::basic_string<char> (const std::basic_string<char> &)> arg1;
            do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
            if (!ok) { break; }
            gaf::GAFAsset* ret = gaf::GAFAsset::create(arg0, arg1);
            object_to_luaval<gaf::GAFAsset>(tolua_S, "gaf.GAFAsset",(gaf::GAFAsset*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d", "gaf.GAFAsset:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_create'.",&tolua_err);
#endif
    return 0;
}
int lua_gaf_GAFAsset_createWithBundle(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"gaf.GAFAsset",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 3)
    {
        std::string arg0;
        std::string arg1;
        std::function<std::basic_string<char> (const std::basic_string<char> &)> arg2;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_createWithBundle'", nullptr);
            return 0;
        }
        gaf::GAFAsset* ret = gaf::GAFAsset::createWithBundle(arg0, arg1, arg2);
        object_to_luaval<gaf::GAFAsset>(tolua_S, "gaf.GAFAsset",(gaf::GAFAsset*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "gaf.GAFAsset:createWithBundle",argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_createWithBundle'.",&tolua_err);
#endif
    return 0;
}
int lua_gaf_GAFAsset_constructor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAsset* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAsset_constructor'", nullptr);
            return 0;
        }
        cobj = new gaf::GAFAsset();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"gaf.GAFAsset");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAsset:GAFAsset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAsset_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_gaf_GAFAsset_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFAsset)");
    return 0;
}

int lua_register_gaf_GAFAsset(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFAsset");
    tolua_cclass(tolua_S,"GAFAsset","gaf.GAFAsset","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"GAFAsset");
        tolua_function(tolua_S,"new",lua_gaf_GAFAsset_constructor);
        tolua_function(tolua_S,"getSceneHeight",lua_gaf_GAFAsset_getSceneHeight);
        tolua_function(tolua_S,"setSceneHeight",lua_gaf_GAFAsset_setSceneHeight);
        tolua_function(tolua_S,"getSceneColor",lua_gaf_GAFAsset_getSceneColor);
        tolua_function(tolua_S,"setSceneColor",lua_gaf_GAFAsset_setSceneColor);
        tolua_function(tolua_S,"getGAFFileName",lua_gaf_GAFAsset_getGAFFileName);
        tolua_function(tolua_S,"createObjectAndRun",lua_gaf_GAFAsset_createObjectAndRun);
        tolua_function(tolua_S,"setDesiredAtlasScale",lua_gaf_GAFAsset_setDesiredAtlasScale);
        tolua_function(tolua_S,"getSceneWidth",lua_gaf_GAFAsset_getSceneWidth);
        tolua_function(tolua_S,"getRootTimeline",lua_gaf_GAFAsset_getRootTimeline);
        tolua_function(tolua_S,"getSceneFps",lua_gaf_GAFAsset_getSceneFps);
        tolua_function(tolua_S,"desiredAtlasScale",lua_gaf_GAFAsset_desiredAtlasScale);
        tolua_function(tolua_S,"initWithGAFBundle",lua_gaf_GAFAsset_initWithGAFBundle);
        tolua_function(tolua_S,"setSceneWidth",lua_gaf_GAFAsset_setSceneWidth);
        tolua_function(tolua_S,"setTextureLoadDelegate",lua_gaf_GAFAsset_setTextureLoadDelegate);
        tolua_function(tolua_S,"initWithGAFFile",lua_gaf_GAFAsset_initWithGAFFile);
        tolua_function(tolua_S,"setSceneFps",lua_gaf_GAFAsset_setSceneFps);
        tolua_function(tolua_S,"createObject",lua_gaf_GAFAsset_createObject);
        tolua_function(tolua_S,"getTextureManager",lua_gaf_GAFAsset_getTextureManager);
        tolua_function(tolua_S,"getTimelineByName",lua_gaf_GAFAsset_getTimelineByName);
        tolua_function(tolua_S,"isAssetVersionPlayable", lua_gaf_GAFAsset_isAssetVersionPlayable);
        tolua_function(tolua_S,"create", lua_gaf_GAFAsset_create);
        tolua_function(tolua_S,"createWithBundle", lua_gaf_GAFAsset_createWithBundle);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFAsset).name();
    g_luaType[typeName] = "gaf.GAFAsset";
    g_typeCast["GAFAsset"] = "gaf.GAFAsset";
    return 1;
}

int lua_gaf_GAFSprite_setLocator(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFSprite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFSprite",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFSprite*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFSprite_setLocator'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFSprite_setLocator'", nullptr);
            return 0;
        }
        cobj->setLocator(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFSprite:setLocator",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFSprite_setLocator'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFSprite_constructor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFSprite* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFSprite_constructor'", nullptr);
            return 0;
        }
        cobj = new gaf::GAFSprite();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"gaf.GAFSprite");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFSprite:GAFSprite",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFSprite_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_gaf_GAFSprite_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFSprite)");
    return 0;
}

int lua_register_gaf_GAFSprite(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFSprite");
    tolua_cclass(tolua_S,"GAFSprite","gaf.GAFSprite","cc.Sprite",nullptr);

    tolua_beginmodule(tolua_S,"GAFSprite");
        tolua_function(tolua_S,"new",lua_gaf_GAFSprite_constructor);
        tolua_function(tolua_S,"setLocator",lua_gaf_GAFSprite_setLocator);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFSprite).name();
    g_luaType[typeName] = "gaf.GAFSprite";
    g_typeCast["GAFSprite"] = "gaf.GAFSprite";
    return 1;
}

int lua_gaf_GAFObject_setAnimationStartedNextLoopDelegate(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setAnimationStartedNextLoopDelegate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<void (gaf::GAFObject *)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setAnimationStartedNextLoopDelegate'", nullptr);
            return 0;
        }
        cobj->setAnimationStartedNextLoopDelegate(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setAnimationStartedNextLoopDelegate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setAnimationStartedNextLoopDelegate'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setAnimationFinishedPlayDelegate(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setAnimationFinishedPlayDelegate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<void (gaf::GAFObject *)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setAnimationFinishedPlayDelegate'", nullptr);
            return 0;
        }
        cobj->setAnimationFinishedPlayDelegate(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setAnimationFinishedPlayDelegate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setAnimationFinishedPlayDelegate'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getBoundingBoxForCurrentFrame(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getBoundingBoxForCurrentFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getBoundingBoxForCurrentFrame'", nullptr);
            return 0;
        }
        cocos2d::Rect ret = cobj->getBoundingBoxForCurrentFrame();
        rect_to_luaval(tolua_S, ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getBoundingBoxForCurrentFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getBoundingBoxForCurrentFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setFps(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setFps'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setFps'", nullptr);
            return 0;
        }
        cobj->setFps(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setFps",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setFps'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getObjectByName(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getObjectByName'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            const gaf::GAFObject* ret = cobj->getObjectByName(arg0);
            object_to_luaval<gaf::GAFObject>(tolua_S, "gaf.GAFObject",(gaf::GAFObject*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            gaf::GAFObject* ret = cobj->getObjectByName(arg0);
            object_to_luaval<gaf::GAFObject>(tolua_S, "gaf.GAFObject",(gaf::GAFObject*)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "gaf.GAFObject:getObjectByName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getObjectByName'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_clearSequence(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_clearSequence'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_clearSequence'", nullptr);
            return 0;
        }
        cobj->clearSequence();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:clearSequence",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_clearSequence'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getIsAnimationRunning(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getIsAnimationRunning'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getIsAnimationRunning'", nullptr);
            return 0;
        }
        bool ret = cobj->getIsAnimationRunning();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getIsAnimationRunning",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getIsAnimationRunning'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_gotoAndStop(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_gotoAndStop'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0);

            if (!ok) { break; }
            bool ret = cobj->gotoAndStop(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            bool ret = cobj->gotoAndStop(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "gaf.GAFObject:gotoAndStop",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_gotoAndStop'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getStartFrame(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getStartFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getStartFrame'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getStartFrame(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getStartFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getStartFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setFramePlayedDelegate(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setFramePlayedDelegate'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::function<void (gaf::GAFObject *, unsigned int)> arg0;

        do {
			// Lambda binding for lua is not supported.
			assert(false);
		} while(0)
		;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setFramePlayedDelegate'", nullptr);
            return 0;
        }
        cobj->setFramePlayedDelegate(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setFramePlayedDelegate",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setFramePlayedDelegate'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getCurrentFrameIndex(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getCurrentFrameIndex'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getCurrentFrameIndex'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getCurrentFrameIndex();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getCurrentFrameIndex",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getCurrentFrameIndex'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getTotalFrameCount(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getTotalFrameCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getTotalFrameCount'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getTotalFrameCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getTotalFrameCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getTotalFrameCount'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_start(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_start'", nullptr);
            return 0;
        }
        cobj->start();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:start",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_start'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_init(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        gaf::GAFAsset* arg0;
        gaf::GAFTimeline* arg1;

        ok &= luaval_to_object<gaf::GAFAsset>(tolua_S, 2, "gaf.GAFAsset",&arg0);

        ok &= luaval_to_object<gaf::GAFTimeline>(tolua_S, 3, "gaf.GAFTimeline",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_init'", nullptr);
            return 0;
        }
        bool ret = cobj->init(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:init",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_init'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_isVisibleInCurrentFrame(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_isVisibleInCurrentFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_isVisibleInCurrentFrame'", nullptr);
            return 0;
        }
        bool ret = cobj->isVisibleInCurrentFrame();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:isVisibleInCurrentFrame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_isVisibleInCurrentFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_isDone(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_isDone'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_isDone'", nullptr);
            return 0;
        }
        bool ret = cobj->isDone();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:isDone",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_isDone'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_playSequence(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_playSequence'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        bool arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_playSequence'", nullptr);
            return 0;
        }
        bool ret = cobj->playSequence(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        bool arg1;
        bool arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);

        ok &= luaval_to_boolean(tolua_S, 4,&arg2);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_playSequence'", nullptr);
            return 0;
        }
        bool ret = cobj->playSequence(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:playSequence",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_playSequence'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_enableTick(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_enableTick'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_enableTick'", nullptr);
            return 0;
        }
        cobj->enableTick(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:enableTick",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_enableTick'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_stop(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_stop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_stop'", nullptr);
            return 0;
        }
        cobj->stop();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:stop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_stop'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setAnimationRunning(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setAnimationRunning'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        bool arg0;
        bool arg1;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setAnimationRunning'", nullptr);
            return 0;
        }
        cobj->setAnimationRunning(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setAnimationRunning",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setAnimationRunning'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_isReversed(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_isReversed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_isReversed'", nullptr);
            return 0;
        }
        bool ret = cobj->isReversed();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:isReversed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_isReversed'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setFrame(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setFrame'", nullptr);
            return 0;
        }
        bool ret = cobj->setFrame(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setLooped(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setLooped'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setLooped'", nullptr);
            return 0;
        }
        cobj->setLooped(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        bool arg0;
        bool arg1;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setLooped'", nullptr);
            return 0;
        }
        cobj->setLooped(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setLooped",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setLooped'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getEndFrame(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getEndFrame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getEndFrame'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getEndFrame(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getEndFrame",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getEndFrame'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_pauseAnimation(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_pauseAnimation'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_pauseAnimation'", nullptr);
            return 0;
        }
        cobj->pauseAnimation();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:pauseAnimation",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_pauseAnimation'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_gotoAndPlay(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_gotoAndPlay'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned int arg0;
            ok &= luaval_to_uint32(tolua_S, 2,&arg0);

            if (!ok) { break; }
            bool ret = cobj->gotoAndPlay(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            bool ret = cobj->gotoAndPlay(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "gaf.GAFObject:gotoAndPlay",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_gotoAndPlay'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_isLooped(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_isLooped'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_isLooped'", nullptr);
            return 0;
        }
        bool ret = cobj->isLooped();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:isLooped",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_isLooped'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_resumeAnimation(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_resumeAnimation'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_resumeAnimation'", nullptr);
            return 0;
        }
        cobj->resumeAnimation();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:resumeAnimation",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_resumeAnimation'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_setReversed(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_setReversed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setReversed'", nullptr);
            return 0;
        }
        cobj->setReversed(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        bool arg0;
        bool arg1;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_setReversed'", nullptr);
            return 0;
        }
        cobj->setReversed(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:setReversed",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_setReversed'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_hasSequences(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_hasSequences'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_hasSequences'", nullptr);
            return 0;
        }
        bool ret = cobj->hasSequences();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:hasSequences",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_hasSequences'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_getFps(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFObject*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFObject_getFps'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_getFps'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getFps();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:getFps",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_getFps'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFObject_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"gaf.GAFObject",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        gaf::GAFAsset* arg0;
        gaf::GAFTimeline* arg1;
        ok &= luaval_to_object<gaf::GAFAsset>(tolua_S, 2, "gaf.GAFAsset",&arg0);
        ok &= luaval_to_object<gaf::GAFTimeline>(tolua_S, 3, "gaf.GAFTimeline",&arg1);
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_create'", nullptr);
            return 0;
        }
        gaf::GAFObject* ret = gaf::GAFObject::create(arg0, arg1);
        object_to_luaval<gaf::GAFObject>(tolua_S, "gaf.GAFObject",(gaf::GAFObject*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "gaf.GAFObject:create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_create'.",&tolua_err);
#endif
    return 0;
}
int lua_gaf_GAFObject_constructor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFObject* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFObject_constructor'", nullptr);
            return 0;
        }
        cobj = new gaf::GAFObject();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"gaf.GAFObject");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFObject:GAFObject",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFObject_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_gaf_GAFObject_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFObject)");
    return 0;
}

int lua_register_gaf_GAFObject(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFObject");
    tolua_cclass(tolua_S,"GAFObject","gaf.GAFObject","gaf.GAFSprite",nullptr);

    tolua_beginmodule(tolua_S,"GAFObject");
        tolua_function(tolua_S,"new",lua_gaf_GAFObject_constructor);
        tolua_function(tolua_S,"setAnimationStartedNextLoopDelegate",lua_gaf_GAFObject_setAnimationStartedNextLoopDelegate);
        tolua_function(tolua_S,"setAnimationFinishedPlayDelegate",lua_gaf_GAFObject_setAnimationFinishedPlayDelegate);
        tolua_function(tolua_S,"getBoundingBoxForCurrentFrame",lua_gaf_GAFObject_getBoundingBoxForCurrentFrame);
        tolua_function(tolua_S,"setFps",lua_gaf_GAFObject_setFps);
        tolua_function(tolua_S,"getObjectByName",lua_gaf_GAFObject_getObjectByName);
        tolua_function(tolua_S,"clearSequence",lua_gaf_GAFObject_clearSequence);
        tolua_function(tolua_S,"getIsAnimationRunning",lua_gaf_GAFObject_getIsAnimationRunning);
        tolua_function(tolua_S,"gotoAndStop",lua_gaf_GAFObject_gotoAndStop);
        tolua_function(tolua_S,"getStartFrame",lua_gaf_GAFObject_getStartFrame);
        tolua_function(tolua_S,"setFramePlayedDelegate",lua_gaf_GAFObject_setFramePlayedDelegate);
        tolua_function(tolua_S,"getCurrentFrameIndex",lua_gaf_GAFObject_getCurrentFrameIndex);
        tolua_function(tolua_S,"getTotalFrameCount",lua_gaf_GAFObject_getTotalFrameCount);
        tolua_function(tolua_S,"start",lua_gaf_GAFObject_start);
        tolua_function(tolua_S,"init",lua_gaf_GAFObject_init);
        tolua_function(tolua_S,"isVisibleInCurrentFrame",lua_gaf_GAFObject_isVisibleInCurrentFrame);
        tolua_function(tolua_S,"isDone",lua_gaf_GAFObject_isDone);
        tolua_function(tolua_S,"playSequence",lua_gaf_GAFObject_playSequence);
        tolua_function(tolua_S,"enableTick",lua_gaf_GAFObject_enableTick);
        tolua_function(tolua_S,"stop",lua_gaf_GAFObject_stop);
        tolua_function(tolua_S,"setAnimationRunning",lua_gaf_GAFObject_setAnimationRunning);
        tolua_function(tolua_S,"isReversed",lua_gaf_GAFObject_isReversed);
        tolua_function(tolua_S,"setFrame",lua_gaf_GAFObject_setFrame);
        tolua_function(tolua_S,"setLooped",lua_gaf_GAFObject_setLooped);
        tolua_function(tolua_S,"getEndFrame",lua_gaf_GAFObject_getEndFrame);
        tolua_function(tolua_S,"pauseAnimation",lua_gaf_GAFObject_pauseAnimation);
        tolua_function(tolua_S,"gotoAndPlay",lua_gaf_GAFObject_gotoAndPlay);
        tolua_function(tolua_S,"isLooped",lua_gaf_GAFObject_isLooped);
        tolua_function(tolua_S,"resumeAnimation",lua_gaf_GAFObject_resumeAnimation);
        tolua_function(tolua_S,"setReversed",lua_gaf_GAFObject_setReversed);
        tolua_function(tolua_S,"hasSequences",lua_gaf_GAFObject_hasSequences);
        tolua_function(tolua_S,"getFps",lua_gaf_GAFObject_getFps);
        tolua_function(tolua_S,"create", lua_gaf_GAFObject_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFObject).name();
    g_luaType[typeName] = "gaf.GAFObject";
    g_typeCast["GAFObject"] = "gaf.GAFObject";
    return 1;
}

int lua_gaf_GAFAssetTextureManager_getMemoryConsumptionStat(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAssetTextureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"gaf.GAFAssetTextureManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (gaf::GAFAssetTextureManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_gaf_GAFAssetTextureManager_getMemoryConsumptionStat'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAssetTextureManager_getMemoryConsumptionStat'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->getMemoryConsumptionStat();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAssetTextureManager:getMemoryConsumptionStat",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAssetTextureManager_getMemoryConsumptionStat'.",&tolua_err);
#endif

    return 0;
}
int lua_gaf_GAFAssetTextureManager_constructor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFAssetTextureManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFAssetTextureManager_constructor'", nullptr);
            return 0;
        }
        cobj = new gaf::GAFAssetTextureManager();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"gaf.GAFAssetTextureManager");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFAssetTextureManager:GAFAssetTextureManager",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFAssetTextureManager_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_gaf_GAFAssetTextureManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFAssetTextureManager)");
    return 0;
}

int lua_register_gaf_GAFAssetTextureManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFAssetTextureManager");
    tolua_cclass(tolua_S,"GAFAssetTextureManager","gaf.GAFAssetTextureManager","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"GAFAssetTextureManager");
        tolua_function(tolua_S,"new",lua_gaf_GAFAssetTextureManager_constructor);
        tolua_function(tolua_S,"getMemoryConsumptionStat",lua_gaf_GAFAssetTextureManager_getMemoryConsumptionStat);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFAssetTextureManager).name();
    g_luaType[typeName] = "gaf.GAFAssetTextureManager";
    g_typeCast["GAFAssetTextureManager"] = "gaf.GAFAssetTextureManager";
    return 1;
}

int lua_gaf_GAFMovieClip_constructor(lua_State* tolua_S)
{
    int argc = 0;
    gaf::GAFMovieClip* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_gaf_GAFMovieClip_constructor'", nullptr);
            return 0;
        }
        cobj = new gaf::GAFMovieClip();
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"gaf.GAFMovieClip");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "gaf.GAFMovieClip:GAFMovieClip",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_gaf_GAFMovieClip_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_gaf_GAFMovieClip_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GAFMovieClip)");
    return 0;
}

int lua_register_gaf_GAFMovieClip(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"gaf.GAFMovieClip");
    tolua_cclass(tolua_S,"GAFMovieClip","gaf.GAFMovieClip","gaf.GAFObject",nullptr);

    tolua_beginmodule(tolua_S,"GAFMovieClip");
        tolua_function(tolua_S,"new",lua_gaf_GAFMovieClip_constructor);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(gaf::GAFMovieClip).name();
    g_luaType[typeName] = "gaf.GAFMovieClip";
    g_typeCast["GAFMovieClip"] = "gaf.GAFMovieClip";
    return 1;
}
TOLUA_API int register_all_gaf(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"gaf",0);
	tolua_beginmodule(tolua_S,"gaf");

	lua_register_gaf_GAFAsset(tolua_S);
	lua_register_gaf_GAFSprite(tolua_S);
	lua_register_gaf_GAFAnimationSequence(tolua_S);
	lua_register_gaf_GAFObject(tolua_S);
	lua_register_gaf_GAFMovieClip(tolua_S);
	lua_register_gaf_GAFAssetTextureManager(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

