# Oatpp 기반 C++ SDK 프로젝트 설계 문서

## 1. 프로젝트 개요

### 1.1 목표
- **단순한 사용성**을 가진 C++ HTTP/WebSocket/SSE SDK 구현
- Oatpp 라이브러리를 래핑하여 이식성 확보
- 복잡한 웹 프레임워크 개념 없이 **API 서빙**과 **실시간 통신**에 집중
- **MCP (Model Context Protocol)** 구현을 위한 특화 기능 제공
- 간단한 핸들러 기반 아키텍처

### 1.2 단계별 구현 계획
1. **Phase 1**: 기본 HTTP 서버와 JSON 처리 (완료)
2. **Phase 2**: WebSocket 및 SSE 기본 기능
3. **Phase 3**: MCP 프로토콜 특화 구현
4. **Phase 4**: SDK 사용성 개선 및 Fluent API

## 2. SDK 아키텍처 설계

### 2.1 단순한 핸들러 기반 구조

```
┌─────────────────────────────────────────┐
│              SDK Public API             │
│         (OatppSDK, Builder)            │
├─────────────────────────────────────────┤
│            Handler Layer                │
│  (HTTP, WebSocket, SSE, MCP Handlers)  │
├─────────────────────────────────────────┤
│           Infrastructure Layer          │
│      (Oatpp Wrappers, JSON Utils)     │
└─────────────────────────────────────────┘
```

### 2.2 SDK 모듈 구조

```
src/
├── sdk/
│   ├── OatppSDK.hpp                    # 메인 SDK 클래스
│   ├── SDKBuilder.hpp                  # Fluent API 빌더
│   └── handlers/
│       ├── HttpHandler.hpp             # HTTP 엔드포인트 핸들러
│       ├── WebSocketHandler.hpp        # WebSocket 메시지 핸들러  
│       ├── SSEHandler.hpp              # SSE 이벤트 핸들러
│       └── MCPHandler.hpp              # MCP 프로토콜 핸들러
├── infrastructure/
│   ├── json/
│   │   ├── Jsonable.hpp                # JSON 직렬화 인터페이스
│   │   └── JsonHelper.hpp              # JSON 유틸리티
│   └── oatpp/
│       ├── OatppServerWrapper.hpp      # 서버 래퍼
│       ├── WebSocketManager.hpp        # WebSocket 연결 관리
│       └── SSEManager.hpp              # SSE 연결 관리
└── examples/
    ├── simple_api_server.cpp           # 기본 API 서버 예제
    ├── websocket_echo.cpp              # WebSocket 에코 서버 예제
    ├── sse_notifications.cpp           # SSE 알림 서버 예제
    └── mcp_server.cpp                  # MCP 서버 예제
```

## 3. 기술 스택

### 3.1 핵심 라이브러리
- **Oatpp**: HTTP/WebSocket 서버 프레임워크
- **RapidJSON**: JSON 처리 (Phase 1.5에서 도입)
- **fmt**: 문자열 포맷팅
- **spdlog**: 로깅

### 3.2 SDK 특징
- **헤더 온리**: 간단한 포함으로 사용 가능
- **정적 링킹**: 의존성 최소화
- **크로스 플랫폼**: Windows/Linux/macOS 지원
- **단순한 API**: 복잡한 설정 없이 즉시 사용

## 4. SDK 사용법

### 4.1 기본 HTTP 서버

```cpp
#include "sdk/OatppSDK.hpp"

int main() {
    auto sdk = OatppSDK::builder()
        .port(8080)
        .build();
    
    // 간단한 GET 엔드포인트
    sdk.addEndpoint("GET", "/api/health", [](const Request& req) {
        return Response::ok("{\"status\":\"OK\"}");
    });
    
    // JSON 응답 자동 처리
    sdk.addJsonEndpoint("GET", "/api/data", [](const Request& req) {
        return JsonResponse{{"message", "Hello World"}};
    });
    
    sdk.start();
    return 0;
}
```

### 4.2 WebSocket 서버

```cpp
#include "sdk/OatppSDK.hpp"

int main() {
    auto sdk = OatppSDK::builder()
        .port(8080)
        .enableWebSocket()
        .build();
    
    // WebSocket 메시지 핸들러
    sdk.addWebSocketHandler("/ws", {
        .onConnect = [](WebSocketClient& client) {
            client.send("{\"type\":\"welcome\"}");
        },
        .onMessage = [](WebSocketClient& client, const std::string& message) {
            // 에코 서버
            client.send("Echo: " + message);
        },
        .onDisconnect = [](WebSocketClient& client) {
            // 정리 작업
        }
    });
    
    sdk.start();
    return 0;
}
```

### 4.3 SSE 서버

```cpp
#include "sdk/OatppSDK.hpp"

int main() {
    auto sdk = OatppSDK::builder()
        .port(8080)
        .enableSSE()
        .build();
    
    // SSE 스트림 엔드포인트
    sdk.addSSEEndpoint("/events", [](SSEClient& client) {
        // 클라이언트 연결 시 호출
        client.send("data", "Connected to events");
    });
    
    // 주기적 이벤트 전송
    sdk.scheduleSSE(std::chrono::seconds(5), [&sdk](auto timestamp) {
        sdk.broadcastSSE("/events", "ping", "Server ping");
    });
    
    sdk.start();
    return 0;
}
```

### 4.4 MCP 서버

```cpp
#include "sdk/OatppSDK.hpp"
#include "sdk/handlers/MCPHandler.hpp"

int main() {
    auto sdk = OatppSDK::builder()
        .port(8080)
        .enableMCP()
        .build();
    
    // MCP 프로토콜 핸들러
    sdk.addMCPHandler({
        .tools = {
            {"get_weather", [](const MCPRequest& req) {
                return MCPResponse::success("Sunny, 25°C");
            }},
            {"send_email", [](const MCPRequest& req) {
                return MCPResponse::success("Email sent");
            }}
        },
        .resources = {
            {"config.json", [](const MCPRequest& req) {
                return MCPResponse::resource("application/json", configData);
            }}
        }
    });
    
    sdk.start();
    return 0;
}
```

## 5. MCP 프로토콜 지원

### 5.1 MCP (Model Context Protocol) 개요
- AI 모델과 컨텍스트 간의 표준화된 통신 프로토콜
- 도구(Tools), 리소스(Resources), 프롬프트(Prompts) 제공
- JSON-RPC 기반 메시지 교환

### 5.2 MCP 메시지 형식

#### 도구 호출 요청
```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "get_weather",
    "arguments": {
      "location": "Seoul"
    }
  },
  "id": 1
}
```

#### 리소스 요청
```json
{
  "jsonrpc": "2.0",
  "method": "resources/read",
  "params": {
    "uri": "file://config.json"
  },
  "id": 2
}
```

### 5.3 SDK에서 MCP 구현
```cpp
// MCP 핸들러 정의
MCPHandler mcpHandler;
mcpHandler.addTool("get_weather", [](const json& args) {
    std::string location = args["location"];
    return json{{"temperature", "25°C"}, {"condition", "sunny"}};
});

mcpHandler.addResource("config", [](const std::string& uri) {
    return ResourceResponse::json(loadConfig());
});

sdk.addMCPHandler("/mcp", mcpHandler);
```

## 6. 에러 처리 전략

### 6.1 간단한 예외 처리
```cpp
// SDK 레벨 예외
class SDKException : public std::exception {};
class ConnectionException : public SDKException {};
class InvalidRequestException : public SDKException {};

// 사용법
try {
    sdk.start();
} catch (const SDKException& e) {
    std::cerr << "SDK Error: " << e.what() << std::endl;
}
```

## 7. SDK 설정 및 빌드

### 7.1 간단한 설정
```cpp
auto sdk = OatppSDK::builder()
    .port(8080)
    .threads(4)                    // 워커 스레드 수
    .enableCORS()                  // CORS 활성화
    .enableLogging(LogLevel::INFO) // 로깅 레벨
    .build();
```

### 7.2 CMake 통합
```cmake
# CMakeLists.txt
find_package(OatppSDK REQUIRED)
target_link_libraries(your_app OatppSDK::OatppSDK)
```

### 7.3 헤더 온리 사용
```cpp
#include "OatppSDK.hpp"  // 모든 기능 포함
// 또는 선택적 포함
#include "OatppSDK/Http.hpp"      // HTTP만
#include "OatppSDK/WebSocket.hpp" // WebSocket만
#include "OatppSDK/SSE.hpp"       // SSE만
#include "OatppSDK/MCP.hpp"       // MCP만
```

## 8. 실제 사용 사례

### 8.1 AI 모델 API 서버
```cpp
auto sdk = OatppSDK::builder().port(8080).build();

sdk.addJsonEndpoint("POST", "/api/chat", [](const Request& req) {
    auto input = req.getJson();
    auto response = callAIModel(input["message"]);
    return JsonResponse{{"response", response}};
});
```

### 8.2 실시간 알림 시스템
```cpp
auto sdk = OatppSDK::builder().enableSSE().build();

sdk.addSSEEndpoint("/notifications", [](SSEClient& client) {
    // 새 클라이언트 등록
    notificationService.subscribe(client);
});

// 알림 전송
notificationService.broadcast("New message arrived!");
```

### 8.3 게임 서버
```cpp
auto sdk = OatppSDK::builder().enableWebSocket().build();

sdk.addWebSocketHandler("/game", {
    .onMessage = [](WebSocketClient& client, const std::string& msg) {
        auto command = parseGameCommand(msg);
        auto result = gameEngine.processCommand(command);
        client.send(result.toJson());
    }
});
``` 