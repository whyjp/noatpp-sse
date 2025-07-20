#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "../../domain/models/HealthStatus.hpp"
#include <spdlog/spdlog.h>

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
};

#include OATPP_CODEGEN_END(ApiController)