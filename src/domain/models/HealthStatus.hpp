#pragma once

#include "../../infrastructure/json/Jsonable.hpp"
#include <string>
#include <ctime>

namespace domain {

class HealthStatus : public json::Jsonable {
private:
    std::string status_;
    std::string message_;
    int64_t timestamp_;
    std::string jsonLibrary_;

public:
    // Constructor
    HealthStatus() 
        : status_("OK")
        , message_("Server is running")
        , timestamp_(std::time(nullptr))
        , jsonLibrary_("RapidJSON") {}
    
    HealthStatus(const std::string& status, const std::string& message)
        : status_(status)
        , message_(message)
        , timestamp_(std::time(nullptr))
        , jsonLibrary_("RapidJSON") {}
    
    // Getter methods
    const std::string& getStatus() const { return status_; }
    const std::string& getMessage() const { return message_; }
    int64_t getTimestamp() const { return timestamp_; }
    const std::string& getJsonLibrary() const { return jsonLibrary_; }
    
    // Setter methods
    void setStatus(const std::string& status) { status_ = status; }
    void setMessage(const std::string& message) { message_ = message; }
    void setTimestamp(int64_t timestamp) { timestamp_ = timestamp; }
    
    // Jsonable interface implementation
    void fromJson(const std::string& jsonStr) override {
        auto doc = parseJson(jsonStr);
        fromDocument(doc);
    }
    
    std::string toJson() const override {
        rapidjson::Document doc;
        doc.SetObject();
        auto value = toValue(doc.GetAllocator());
        doc.CopyFrom(value, doc.GetAllocator());
        return valueToString(doc);
    }
    
    void fromDocument(const rapidjson::Value& value) override {
        if (!value.IsObject()) {
            throw std::runtime_error("HealthStatus: JSON value is not an object");
        }
        
        status_ = getString(value, "status", "OK");
        message_ = getString(value, "message", "Server is running");
        timestamp_ = getInt64(value, "timestamp", std::time(nullptr));
        jsonLibrary_ = getString(value, "json_library", "RapidJSON");
    }
    
    rapidjson::Value toValue(rapidjson::Document::AllocatorType& allocator) const override {
        rapidjson::Value obj(rapidjson::kObjectType);
        
        obj.AddMember("status", rapidjson::Value(status_.c_str(), allocator), allocator);
        obj.AddMember("message", rapidjson::Value(message_.c_str(), allocator), allocator);
        obj.AddMember("timestamp", timestamp_, allocator);
        obj.AddMember("json_library", rapidjson::Value(jsonLibrary_.c_str(), allocator), allocator);
        
        return obj;
    }
};

} // namespace domain