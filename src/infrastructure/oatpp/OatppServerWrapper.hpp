#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/core/macro/component.hpp"
#include <memory>
#include <fstream>

class OatppServerWrapper {
private:
    std::shared_ptr<oatpp::network::ServerConnectionProvider> connectionProvider;
    std::shared_ptr<oatpp::web::server::HttpConnectionHandler> connectionHandler;
    std::shared_ptr<oatpp::network::Server> server;
    
    int port;
    
public:
    explicit OatppServerWrapper(int port = 8080);
    ~OatppServerWrapper();
    
    void initialize();
    void start();
    void stop();
    
    int getPort() const { return port; }
    
private:
    int findAvailablePort(int startPort = 8080, int maxPort = 9080);
    void writePortToFile(int port);
    bool isPortAvailable(int port);
};