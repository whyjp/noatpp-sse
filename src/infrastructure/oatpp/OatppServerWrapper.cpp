#include "OatppServerWrapper.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "../../presentation/controllers/HttpController.hpp"

OatppServerWrapper::OatppServerWrapper(int port) : port(port) {
}

OatppServerWrapper::~OatppServerWrapper() {
    stop();
}

void OatppServerWrapper::initialize() {
    // Initialize oatpp environment
    oatpp::base::Environment::init();
    
    // Create router for HTTP requests
    auto router = oatpp::web::server::HttpRouter::createShared();
    
    // Create and add controller
    auto httpController = HttpController::createShared();
    router->addController(httpController);
    
    // Create connection handler
    connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    
    // Create connection provider
    connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
        {"localhost", static_cast<v_uint16>(port), oatpp::network::Address::IP_4}
    );
}

void OatppServerWrapper::start() {
    if (!connectionProvider || !connectionHandler) {
        initialize();
    }
    
    // Create server
    server = oatpp::network::Server::createShared(connectionProvider, connectionHandler);
    
    OATPP_LOGI("Server", "Starting server on port %d", port);
    
    // Start server
    server->run();
}

void OatppServerWrapper::stop() {
    if (server) {
        server->stop();
        server.reset();
    }
    
    connectionHandler.reset();
    connectionProvider.reset();
    
    // Destroy oatpp environment
    oatpp::base::Environment::destroy();
}