#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include <ctime>

#include OATPP_CODEGEN_BEGIN(ApiController)

class HttpController : public oatpp::web::server::api::ApiController {
public:
    HttpController() 
        : oatpp::web::server::api::ApiController(oatpp::parser::json::mapping::ObjectMapper::createShared()) {}
    
    static std::shared_ptr<HttpController> createShared() {
        return std::make_shared<HttpController>();
    }
    
    ENDPOINT("GET", "/api/health", health) {
        // Simple JSON response without DTO
        auto response = createResponse(Status::CODE_200, 
            "{\"status\":\"OK\",\"message\":\"Server is running\",\"timestamp\":" + 
            std::to_string(std::time(nullptr)) + "}");
        response->putHeader(Header::CONTENT_TYPE, "application/json");
        return response;
    }
    
    ENDPOINT("GET", "/", root) {
        return createResponse(Status::CODE_200, "Oatpp SSE Server is running!");
    }
};

#include OATPP_CODEGEN_END(ApiController)