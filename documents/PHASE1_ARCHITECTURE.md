# 1단계 아키텍처 문서 (Phase 1.5 포함)

## 개요

1단계는 oatpp 프레임워크를 사용하여 크로스 플랫폼 빌드 지원과 정적 링킹을 갖춘 기본 HTTP 서버를 구현합니다. Phase 1.5에서는 RapidJSON 라이브러리와 Jsonable 인터페이스 패턴을 도입하여 더 깔끔한 JSON 처리를 구현했습니다.

## 아키텍처 다이어그램

```mermaid
classDiagram
    %% Phase 1.5: Jsonable 인터페이스 패턴 포함
    
    class main {
        <<진입점>>
        +main()
        +signalHandler()
    }

    class OatppServerWrapper {
        <<서버 래퍼>>
        +initialize()
        +start()
        +stop()
    }

    class HttpController {
        <<HTTP 컨트롤러>>
        +health()
        +root()
    }

    class Jsonable {
        <<인터페이스>>
        +fromJson(string)
        +toJson() string
        +fromDocument(Value)
        +toValue(Allocator) Value
    }

    class HealthStatus {
        <<도메인 모델>>
        -status: string
        -message: string
        -timestamp: int64
        -jsonLibrary: string
        +getStatus() string
        +getMessage() string
    }

    class RapidJSON {
        <<외부 라이브러리>>
        Document, Value
        Writer, Reader
    }

    class OatppFramework {
        <<외부 프레임워크>>
        Router, Server, Handler
        ObjectMapper
    }

    %% 의존 관계
    main --> OatppServerWrapper : 생성/소유
    OatppServerWrapper --> HttpController : 등록
    OatppServerWrapper --> OatppFramework : 직접 사용
    HttpController --> OatppFramework : 직접 상속
    HttpController --> HealthStatus : 사용
    HealthStatus --|> Jsonable : 구현
    Jsonable --> RapidJSON : 내부에서만 사용

    %% 주석
    note for main "애플리케이션 시작점"
    note for OatppServerWrapper "oatpp 프레임워크 감싸기"
    note for HttpController "REST API 엔드포인트"
    note for Jsonable "JSON 처리 추상화"
    note for HealthStatus "헬스 상태 데이터"
    note for RapidJSON "캡슐화된 JSON 라이브러리"
```

### Phase 1.5 주요 특징
- **Jsonable 인터페이스**: JSON 라이브러리 구현 세부사항 캡슐화
- **RapidJSON 통합**: 성능과 메모리 효율성 개선
- **도메인 모델**: HealthStatus 클래스로 비즈니스 로직 분리
- **Interface Segregation**: 다른 클래스들이 RapidJSON을 직접 사용하지 않음
- **깔끔한 아키텍처**: JSON 처리 로직의 명확한 분리

## 파일 구조

```
src/
├── main.cpp                                    # 애플리케이션 진입점
├── domain/models/
│   └── HealthStatus.hpp                        # 헬스 상태 도메인 모델
├── infrastructure/
│   ├── json/
│   │   ├── Jsonable.hpp                        # JSON 직렬화 인터페이스
│   │   └── JsonHelper.hpp                      # JSON 유틸리티 헬퍼
│   └── oatpp/
│       ├── OatppServerWrapper.hpp              # 서버 래퍼 인터페이스
│       └── OatppServerWrapper.cpp              # 서버 래퍼 구현
└── presentation/controllers/
    ├── HttpController.hpp                      # HTTP 컨트롤러 인터페이스
    └── HttpController.cpp                      # HTTP 컨트롤러 구현
```

## 클래스 책임

### 1. main.cpp
- **목적**: 애플리케이션 생명주기 관리
- **책임**:
  - 서버 인스턴스 생성 및 관리
  - 시그널 처리 (SIGINT, SIGTERM)
  - 우아한 종료
- **의존성**: OatppServerWrapper

### 2. OatppServerWrapper
- **목적**: Oatpp 프레임워크 추상화
- **책임**:
  - 서버 컴포넌트 초기화
  - 연결 제공자 설정
  - 요청 핸들러 구성
  - 서버 생명주기 관리
- **의존성**: oatpp 프레임워크 클래스들, HttpController

### 3. HttpController
- **목적**: HTTP 엔드포인트 구현
- **책임**:
  - REST API 엔드포인트 정의
  - 요청 처리
  - 응답 생성
- **의존성**: oatpp::ApiController, HealthStatus

### 4. Jsonable (새로 추가 - Phase 1.5)
- **목적**: JSON 직렬화/역직렬화 인터페이스 제공
- **책임**:
  - JSON 문자열 ↔ 객체 변환 인터페이스 정의
  - RapidJSON 구현 세부사항 캡슐화
  - 안전한 JSON 파싱 헬퍼 메서드 제공
- **의존성**: RapidJSON (내부적으로만)

### 5. HealthStatus (새로 추가 - Phase 1.5)
- **목적**: 서버 상태 정보 도메인 모델
- **책임**:
  - 서버 상태 데이터 캡슐화
  - JSON 직렬화/역직렬화 구현
  - 비즈니스 로직과 데이터 관리
- **의존성**: Jsonable 인터페이스

## 관계

### 소유 관계 (강한 결합)
1. `main`이 `OatppServerWrapper`를 소유 (unique_ptr)
2. `OatppServerWrapper`가 oatpp 컴포넌트들을 소유 (shared_ptr)

### 사용 관계 (약한 결합)
1. `OatppServerWrapper`가 라우팅을 위해 `HttpController` 사용
2. `HttpController`가 JSON 처리를 위해 `ObjectMapper` 사용
3. 모든 클래스가 메모리 관리를 위해 스마트 포인터 사용

### 의존성
1. `HttpConnectionHandler`가 `HttpRouter`에 의존
2. `Server`가 `ServerConnectionProvider`와 `HttpConnectionHandler`에 의존
3. `HttpController`가 `oatpp::ApiController`를 상속

## API 엔드포인트

### GET /
- **설명**: 루트 엔드포인트
- **응답**: 일반 텍스트 "Oatpp SSE Server is running!"
- **상태**: 200 OK

### GET /api/health
- **설명**: 헬스 체크 엔드포인트
- **응답**: 서버 상태가 포함된 JSON (HealthStatus 모델 사용)
- **형식**:
```json
{
  "status": "OK",
  "message": "Server is running",
  "timestamp": 1752975680,
  "json_library": "RapidJSON"
}
```
- **상태**: 200 OK
- **변경사항**: Phase 1.5에서 `json_library` 필드 추가 및 Jsonable 인터페이스 사용

## 빌드 구성

### 정적 링킹
- 모든 서드파티 라이브러리 정적 링크
- 런타임 의존성 최소화
- 실행 파일 크기: ~1.5MB (Linux)

### 크로스 플랫폼 지원
- Windows: Visual Studio 2022, 정적 런타임 (/MT)
- Linux: GCC 10+, 정적 라이브러리
- 분리된 빌드 디렉토리 (build-windows/, build-linux/)

### 의존성
- **oatpp**: HTTP 서버 프레임워크
- **RapidJSON**: JSON 처리 (헤더 전용) - Phase 1.5에서 nlohmann/json 교체
- **fmt**: 문자열 포맷팅
- **spdlog**: 로깅 프레임워크

## 1단계 성과 (Phase 1.5 포함)

✅ **기본 HTTP 서버**: 헬스 엔드포인트가 있는 기능적인 웹 서버  
✅ **크로스 플랫폼 빌드**: Windows 및 Linux 지원  
✅ **정적 링킹**: 자체 포함 실행 파일  
✅ **깔끔한 코드 구조**: 관심사의 분리  
✅ **메모리 안전성**: 스마트 포인터 사용  
✅ **오류 처리**: 우아한 종료 및 오류 관리  
✅ **Jsonable 인터페이스**: JSON 라이브러리 캡슐화 (Phase 1.5)  
✅ **RapidJSON 통합**: 성능 개선된 JSON 처리 (Phase 1.5)  
✅ **도메인 모델**: HealthStatus를 통한 비즈니스 로직 분리 (Phase 1.5)  

## 향후 단계

### 2단계: WebSocket 지원
- WebSocket 엔드포인트 추가
- JSON-RPC 2.0 프로토콜 구현
- 구독 관리 추가

### 3단계: SSE 구현
- Server-Sent Events 엔드포인트
- 이벤트 스트리밍 기능
- 클라이언트 연결 관리

### 4단계: Clean Architecture
- 도메인 레이어 분리
- 애플리케이션 서비스
- 리포지토리 패턴
- 의존성 주입

## 기술 노트

### 메모리 관리
- 전체적으로 RAII 패턴 적용
- 자동 정리를 위한 스마트 포인터
- 수동 메모리 관리 없음

### 오류 처리
- 예외 기반 오류 처리
- 우아한 성능 저하
- 시그널 기반 종료

### 스레딩
- 단일 스레드 설계 (1단계)
- oatpp가 내부 스레딩 처리
- 스레드 안전한 서버 작업

### 보안 고려사항
- 컨트롤러에서 입력 검증
- 안전한 문자열 처리
- 디버그 정보 노출 없음

## 빌드 명령어

```bash
# Linux
./build.sh [clean]
./run.sh

# Windows  
build.bat [clean]
run.bat

# 정적 빌드 (Linux)
./build-static.sh [clean]
```

이 아키텍처는 단순함과 신뢰성을 유지하면서 더 복잡한 기능을 구축하기 위한 견고한 기반을 제공합니다.