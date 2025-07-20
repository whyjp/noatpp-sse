#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <string>
#include <stdexcept>

namespace json {

// JSON 직렬화/역직렬화 인터페이스
class Jsonable {
public:
    virtual ~Jsonable() = default;
    
    // JSON 문자열에서 객체로 변환
    virtual void fromJson(const std::string& jsonStr) = 0;
    
    // 객체를 JSON 문자열로 변환
    virtual std::string toJson() const = 0;
    
    // RapidJSON Document에서 객체로 변환
    virtual void fromDocument(const rapidjson::Value& value) = 0;
    
    // 객체를 RapidJSON Value로 변환
    virtual rapidjson::Value toValue(rapidjson::Document::AllocatorType& allocator) const = 0;

protected:
    // JSON 파싱 헬퍼
    static rapidjson::Document parseJson(const std::string& jsonStr) {
        rapidjson::Document doc;
        doc.Parse(jsonStr.c_str());
        
        if (doc.HasParseError()) {
            throw std::runtime_error("JSON 파싱 오류: " + 
                std::string(rapidjson::GetParseError_En(doc.GetParseError())));
        }
        
        return doc;
    }
    
    // JSON 문자열 생성 헬퍼
    static std::string valueToString(const rapidjson::Value& value) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        value.Accept(writer);
        return buffer.GetString();
    }
    
    // 안전한 문자열 추출
    static std::string getString(const rapidjson::Value& value, const char* key, const std::string& defaultValue = "") {
        if (value.HasMember(key) && value[key].IsString()) {
            return value[key].GetString();
        }
        return defaultValue;
    }
    
    // 안전한 정수 추출
    static int64_t getInt64(const rapidjson::Value& value, const char* key, int64_t defaultValue = 0) {
        if (value.HasMember(key) && value[key].IsInt64()) {
            return value[key].GetInt64();
        }
        return defaultValue;
    }
    
    // 안전한 불린 추출
    static bool getBool(const rapidjson::Value& value, const char* key, bool defaultValue = false) {
        if (value.HasMember(key) && value[key].IsBool()) {
            return value[key].GetBool();
        }
        return defaultValue;
    }
    
    // 안전한 배열 확인
    static bool isArray(const rapidjson::Value& value, const char* key) {
        return value.HasMember(key) && value[key].IsArray();
    }
    
    // 안전한 객체 확인
    static bool isObject(const rapidjson::Value& value, const char* key) {
        return value.HasMember(key) && value[key].IsObject();
    }
};

} // namespace json