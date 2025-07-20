#include "infrastructure/oatpp/OatppServerWrapper.hpp"
#include <iostream>
#include <csignal>
#include <memory>

std::unique_ptr<OatppServerWrapper> serverInstance = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Shutting down server..." << std::endl;
    if (serverInstance) {
        serverInstance->stop();
        serverInstance.reset();
    }
    exit(0);
}

int main() {
    // Setup signal handlers for graceful shutdown
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    try {
        std::cout << "Starting Oatpp SSE Server..." << std::endl;
        
        // Create and start server
        serverInstance = std::make_unique<OatppServerWrapper>(8081);
        serverInstance->initialize();
        
        std::cout << "Server initialized on port " << serverInstance->getPort() << std::endl;
        std::cout << "Visit: http://localhost:8081/ or http://localhost:8081/api/health" << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        
        // Start server (this will block)
        serverInstance->start();
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}