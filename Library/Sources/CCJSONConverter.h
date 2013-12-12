#pragma once

#ifndef __CCJSONCONVERTER__H__
#define __CCJSONCONVERTER__H__

#include "CCJSONData.h"

class cJSON;

class CCJSONConverter : public CCObject {
    
public:
    static CCJSONConverter * sharedConverter();
    
    char * strFrom(CCDictionary * dictionary);
    
    CCDictionary * dictionaryFrom(const char * str);
    
private:
    void convertJsonToDictionary(cJSON *json, CCDictionary *dictionary);
    
    void convertDictionaryToJson(CCDictionary *dictionary, cJSON *json);
    
    void convertJsonToArray(cJSON * json, CCArray * array);
    
    void convertArrayToJson(CCArray * array, cJSON * json);
    
    cJSON * getObjJson(CCObject * obj);
    
    CCObject * getJsonObj(cJSON * json);
};

#endif // #ifndef __CCJSONCONVERTER__H__