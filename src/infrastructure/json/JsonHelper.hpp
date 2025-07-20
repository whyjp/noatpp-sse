#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <string>
#include <stdexcept>

namespace json {

class JsonHelper {
public:
    // JSON 문자열을 Document로 파싱
    static rapidjson::Document parse(const std::string& jsonStr) {
        rapidjson::Document doc;
        doc.Parse(jsonStr.c_str());
        
        if (doc.HasParseError()) {
            throw std::runtime_error("JSON 파싱 오류: " + 
                std::string(rapidjson::GetParseError_En(doc.GetParseError())));
        }
        
        return doc;
    }
    
    // Document를 JSON 문자열로 변환
    static std::string stringify(const rapidjson::Document& doc) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);
        return buffer.GetString();
    }
    
    // Object 생성 헬퍼
    static rapidjson::Document createObject() {
        rapidjson::Document doc;
        doc.SetObject();
        return doc;
    }
    
    // Array 생성 헬퍼
    static rapidjson::Document createArray() {
        rapidjson::Document doc;
        doc.SetArray();
        return doc;
    }
};

// JSON 직렬화/역직렬화 인터페이스
template<typename T>
class JsonSerializable {
public:
    virtual ~JsonSerializable() = default;
    
    // C++ 객체를 JSON 문자열로 변환
    virtual std::string toJson() const = 0;
    
    // JSON 문자열에서 C++ 객체로 변환
    virtual void fromJson(const std::string& jsonStr) = 0;
    
    // Document에서 객체로 변환
    virtual void fromDocument(const rapidjson::Document& doc) = 0;
    
    // 객체를 Document로 변환
    virtual rapidjson::Document toDocument() const = 0;
};

} // namespace json