#include "infrastructure/oatpp/OatppServerWrapper.hpp"
#include <iostream>
#include <csignal>
#include <memory>
#include <spdlog/spdlog.h>

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
    
    // Initialize spdlog with info level
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
    
    try {
        std::cout << "Starting Oatpp SSE Server..." << std::endl;
        
        // Create and start server (will find available port automatically)
        serverInstance = std::make_unique<OatppServerWrapper>(8080);
        serverInstance->initialize();
        
        int actualPort = serverInstance->getPort();
        std::cout << "Server initialized on port " << actualPort << std::endl;
        std::cout << "Visit: http://localhost:" << actualPort << "/ or http://localhost:" << actualPort << "/api/health" << std::endl;
        std::cout << "Test page: http://localhost:" << actualPort << "/test" << std::endl;
        std::cout << "Press Ctrl+C to stop the server" << std::endl;
        
        // Start server (this will block)
        serverInstance->start();
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}