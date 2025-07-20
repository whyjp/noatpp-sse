#include "OatppServerWrapper.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "../../presentation/controllers/HttpController.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

OatppServerWrapper::OatppServerWrapper(int port) {
    if (port == 8080) {
        // Find available port in range 8080-9080
        this->port = findAvailablePort();
        spdlog::info("Using available port: {}", this->port);
    } else {
        this->port = port;
    }
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
    
    // Write port info to file
    writePortToFile(port);
    
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

bool OatppServerWrapper::isPortAvailable(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        return false;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    int result = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    close(sock);
    
    return result == 0;
}

int OatppServerWrapper::findAvailablePort(int startPort, int maxPort) {
    for (int port = startPort; port <= maxPort; ++port) {
        if (isPortAvailable(port)) {
            spdlog::info("Found available port: {}", port);
            return port;
        }
    }
    
    spdlog::warn("No available ports found in range {}-{}, using default {}", startPort, maxPort, startPort);
    return startPort;
}

void OatppServerWrapper::writePortToFile(int port) {
    try {
        std::ofstream file("http.txt");
        if (file.is_open()) {
            file << "port:" << port << std::endl;
            file.close();
            spdlog::info("Port info written to http.txt: port:{}", port);
        } else {
            spdlog::error("Failed to create http.txt file");
        }
    } catch (const std::exception& e) {
        spdlog::error("Error writing port file: {}", e.what());
    }
}