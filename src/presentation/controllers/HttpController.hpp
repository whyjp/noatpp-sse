#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "../../domain/models/HealthStatus.hpp"
#include <spdlog/spdlog.h>
#include <fstream>
#include <sstream>

#include OATPP_CODEGEN_BEGIN(ApiController)

class HttpController : public oatpp::web::server::api::ApiController {
public:
    HttpController() 
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {}
    
    static std::shared_ptr<HttpController> createShared() {
        return std::make_shared<HttpController>();
    }
    
    ENDPOINT("GET", "/api/health", health) {
        spdlog::info("REQUEST: GET /api/health");
        
        // Create clean JSON response using Jsonable interface
        domain::HealthStatus healthStatus;
        std::string jsonResponse = healthStatus.toJson();
        
        auto response = createResponse(Status::CODE_200, jsonResponse);
        response->putHeader(Header::CONTENT_TYPE, "application/json");
        
        spdlog::info("RESPONSE: /api/health -> 200 | Body: {}", jsonResponse);
        return response;
    }
    
    ENDPOINT("GET", "/", root) {
        spdlog::info("REQUEST: GET /");
        
        std::string responseBody = "Oatpp SSE Server is running!";
        auto response = createResponse(Status::CODE_200, responseBody);
        
        spdlog::info("RESPONSE: / -> 200 | Body: {}", responseBody);
        return response;
    }
    
    ENDPOINT("GET", "/test", test) {
        spdlog::info("REQUEST: GET /test");
        
        try {
            std::ifstream file("static/test.html");
            if (!file.is_open()) {
                spdlog::error("Failed to open static/test.html");
                auto response = createResponse(Status::CODE_404, "Test page not found");
                spdlog::info("RESPONSE: /test -> 404 | Test page not found");
                return response;
            }
            
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string htmlContent = buffer.str();
            file.close();
            
            auto response = createResponse(Status::CODE_200, htmlContent);
            response->putHeader(Header::CONTENT_TYPE, "text/html; charset=utf-8");
            
            spdlog::info("RESPONSE: /test -> 200 | HTML page served ({}B)", htmlContent.length());
            return response;
            
        } catch (const std::exception& e) {
            spdlog::error("Error serving test page: {}", e.what());
            auto response = createResponse(Status::CODE_500, "Internal server error");
            spdlog::info("RESPONSE: /test -> 500 | Internal server error");
            return response;
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)