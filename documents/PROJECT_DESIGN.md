# Oatpp 기반 WebSocket/SSE 서버 프로젝트 설계 문서

## 1. 프로젝트 개요

### 1.1 목표
- KISS/SOLID 원칙과 Clean Architecture를 적용한 C++ 웹 서버 구현
- Oatpp 라이브러리를 래핑하여 이식성 확보
- HTTP, WebSocket, SSE를 통한 실시간 통신 지원
- JSON-RPC 2.0 기반 구독/구독해제 패턴 구현

### 1.2 단계별 구현 계획
1. **1단계**: 기본 Oatpp 서버 구현
2. **2단계**: League of Legends API 프록시 구현
3. **3단계**: MCP 2025 원격 프로토콜 구현

## 2. 아키텍처 설계

### 2.1 Clean Architecture 레이어 구조

```
┌─────────────────────────────────────────┐
│           Presentation Layer            │
│  (Controllers, WebSocket Handlers)     │
├─────────────────────────────────────────┤
│           Application Layer             │
│     (Use Cases, Services)              │
├─────────────────────────────────────────┤
│            Domain Layer                 │
│    (Entities, Value Objects)           │
├─────────────────────────────────────────┤
│          Infrastructure Layer          │
│   (Oatpp Wrappers, External APIs)     │
└─────────────────────────────────────────┘
```

### 2.2 모듈 구조

```
src/
├── domain/
│   ├── entities/
│   │   ├── Subscription.hpp
│   │   └── Message.hpp
│   ├── repositories/
│   │   ├── ISubscriptionRepository.hpp
│   │   └── IMessageRepository.hpp
│   └── services/
│       └── INotificationService.hpp
├── application/
│   ├── usecases/
│   │   ├── SubscriptionUseCase.hpp
│   │   └── MessageUseCase.hpp
│   └── services/
│       └── NotificationService.hpp
├── infrastructure/
│   ├── oatpp/
│   │   ├── OatppServerWrapper.hpp
│   │   ├── OatppControllerWrapper.hpp
│   │   └── OatppWebSocketWrapper.hpp
│   ├── repositories/
│   │   ├── InMemorySubscriptionRepository.hpp
│   │   └── InMemoryMessageRepository.hpp
│   └── external/
│       └── LoLApiClient.hpp
└── presentation/
    ├── controllers/
    │   ├── HttpController.hpp
    │   ├── WebSocketController.hpp
    │   └── SSEController.hpp
    ├── handlers/
    │   ├── JsonRpcHandler.hpp
    │   └── SubscriptionHandler.hpp
    └── main.cpp
```

## 3. 기술 스택

### 3.1 핵심 라이브러리
- **Oatpp**: HTTP/WebSocket 서버 프레임워크
- **nlohmann/json**: JSON 처리
- **fmt**: 문자열 포맷팅
- **spdlog**: 로깅

### 3.2 빌드 시스템
- **CMake**: 빌드 관리
- **vcpkg**: 패키지 관리 (Windows)

## 4. API 명세

### 4.1 HTTP REST API

#### 4.1.1 기본 엔드포인트
```
GET  /api/health          - 서버 상태 확인
GET  /api/subscriptions   - 구독 목록 조회
POST /api/subscriptions   - 구독 생성
DELETE /api/subscriptions/{id} - 구독 삭제
```

#### 4.1.2 SSE 엔드포인트
```
GET /api/events           - SSE 스트림 연결
GET /api/events/{topic}   - 특정 토픽 SSE 스트림
```

### 4.2 WebSocket API

#### 4.2.1 JSON-RPC 2.0 메시지 포맷
```json
{
  "jsonrpc": "2.0",
  "method": "subscribe",
  "params": {
    "topic": "game_events",
    "filters": {}
  },
  "id": 1
}
```

#### 4.2.2 지원 메서드
- `subscribe`: 토픽 구독
- `unsubscribe`: 토픽 구독 해제
- `list_subscriptions`: 구독 목록 조회

### 4.3 League of Legends API 프록시

#### 4.3.1 프록시 엔드포인트
```
GET /lol/summoner/v4/summoners/by-name/{summonerName}
GET /lol/match/v5/matches/by-puuid/{puuid}/ids
GET /lol/match/v5/matches/{matchId}
```

## 5. 데이터 모델

### 5.1 Subscription Entity
```cpp
class Subscription {
private:
    std::string id_;
    std::string topic_;
    std::string clientId_;
    std::chrono::system_clock::time_point createdAt_;
    std::map<std::string, std::string> filters_;

public:
    // SOLID 원칙 적용: 불변성 보장
    const std::string& getId() const;
    const std::string& getTopic() const;
    const std::string& getClientId() const;
    // ... 기타 getter 메서드
};
```

### 5.2 Message Entity
```cpp
class Message {
private:
    std::string id_;
    std::string topic_;
    std::string payload_;
    std::chrono::system_clock::time_point timestamp_;

public:
    // Value Object 패턴 적용
    const std::string& getId() const;
    const std::string& getTopic() const;
    const std::string& getPayload() const;
    // ... 기타 getter 메서드
};
```

## 6. 의존성 주입 설계

### 6.1 DI Container 구조
```cpp
class DIContainer {
private:
    std::shared_ptr<ISubscriptionRepository> subscriptionRepo_;
    std::shared_ptr<IMessageRepository> messageRepo_;
    std::shared_ptr<INotificationService> notificationService_;

public:
    template<typename T>
    std::shared_ptr<T> resolve();
    
    void configure();
};
```

## 7. 에러 처리 전략

### 7.1 예외 계층 구조
```cpp
// 도메인 예외
class DomainException : public std::exception {};
class SubscriptionNotFoundException : public DomainException {};

// 애플리케이션 예외
class ApplicationException : public std::exception {};
class ValidationException : public ApplicationException {};

// 인프라 예외
class InfrastructureException : public std::exception {};
class NetworkException : public InfrastructureException {};
```

## 8. 로깅 및 모니터링

### 8.1 로깅 레벨
- **TRACE**: 상세한 실행 흐름
- **DEBUG**: 디버깅 정보
- **INFO**: 일반적인 정보
- **WARN**: 경고 메시지
- **ERROR**: 오류 메시지

### 8.2 메트릭 수집
- 활성 WebSocket 연결 수
- 구독 수
- 메시지 처리량
- 응답 시간

## 9. 테스트 전략

### 9.1 테스트 피라미드
- **Unit Tests**: 도메인 로직 테스트
- **Integration Tests**: 레이어 간 통합 테스트
- **E2E Tests**: 전체 시스템 테스트

### 9.2 Mock 객체 사용
```cpp
class MockSubscriptionRepository : public ISubscriptionRepository {
    // Mock 구현
};

class MockNotificationService : public INotificationService {
    // Mock 구현
};
```

## 10. 성능 최적화

### 10.1 연결 풀링
- HTTP 연결 재사용
- WebSocket 연결 관리
- 데이터베이스 연결 풀

### 10.2 메모리 관리
- 스마트 포인터 사용
- RAII 패턴 적용
- 메모리 누수 방지

## 11. 보안 고려사항

### 11.1 인증 및 권한
- JWT 토큰 기반 인증
- 역할 기반 접근 제어
- Rate Limiting

### 11.2 데이터 보호
- HTTPS 강제
- 입력 데이터 검증
- SQL 인젝션 방지

## 12. 배포 및 운영

### 12.1 컨테이너화
- Docker 이미지 생성
- Multi-stage 빌드
- 헬스 체크 구현

### 12.2 설정 관리
- 환경별 설정 분리
- 민감한 정보 암호화
- 동적 설정 로드 