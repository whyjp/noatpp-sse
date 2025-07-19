# Oatpp SSE 서버

WebSocket과 SSE를 지원하는 간단한 oatpp 기반 HTTP 서버입니다.

## 사전 요구사항

- CMake 3.20+
- C++20 호환 컴파일러
- Git

### Windows
- Visual Studio 2022 이상
- Git for Windows

### Linux/Unix
- GCC 10+ 또는 Clang 12+
- Make

## 빠른 시작

### Windows
```batch
# 클린 빌드 (선택사항)
build.bat clean

# 빌드
build.bat

# 서버 실행
run.bat
```

### Linux/Unix
```bash
# 클린 빌드 (선택사항)
chmod +x *.sh
./build.sh clean

# 빌드
./build.sh

# 서버 실행
./run.sh
```

## 빌드 디렉토리 구조

빌드 시스템은 각 플랫폼별로 별도의 디렉토리를 생성합니다:
- `build-windows/` - Windows 빌드 파일 및 실행 파일
- `build-linux/` - Linux 빌드 파일 및 실행 파일

## 사용 가능한 스크립트

### 빌드 스크립트
- `build.sh [clean]` - Linux 빌드 스크립트
- `build.bat [clean]` - Windows 빌드 스크립트

### 실행 스크립트  
- `run.sh` - Linux 서버 시작
- `run.bat` - Windows 서버 시작

### 정리 스크립트
- `clean.sh` - 모든 빌드 디렉토리 정리
- `clean.bat` - 모든 빌드 디렉토리 정리

### 의존성 스크립트
- `setup_dependencies.sh` - 의존성 다운로드 (Linux)
- `setup_dependencies.bat` - 의존성 다운로드 (Windows)

## 엔드포인트

- `GET /` - 루트 엔드포인트: "Oatpp SSE Server is running!"
- `GET /api/health` - 헬스 체크 엔드포인트: 상태, 메시지, 타임스탬프가 포함된 JSON

## 서버 구성

- **포트**: 8080
- **호스트**: localhost
- **엔드포인트**: http://localhost:8080/ 및 http://localhost:8080/api/health

## 프로젝트 구조

```
├── src/
│   ├── infrastructure/oatpp/     # Oatpp 래퍼 클래스들
│   ├── presentation/controllers/ # HTTP 컨트롤러들
│   └── main.cpp                  # 애플리케이션 진입점
├── documents/                    # 프로젝트 문서
│   ├── PROJECT_DESIGN.md        # 전체 프로젝트 설계
│   ├── BUILD_INSTRUCTIONS.md    # 상세 빌드 가이드
│   └── PHASE1_ARCHITECTURE.md   # 1단계 아키텍처 문서
├── third_party/                  # 외부 의존성 (자동 다운로드)
├── build-windows/                # Windows 빌드 출력
├── build-linux/                  # Linux 빌드 출력
├── CMakeLists.txt               # 빌드 구성
├── *.sh, *.bat                  # 빌드/실행 스크립트
└── README.md                    # 이 파일
```

## 개발

1. **클린 빌드**: `./build.sh clean` 또는 `build.bat clean`
2. **빌드**: `./build.sh` 또는 `build.bat`  
3. **실행**: `./run.sh` 또는 `run.bat`
4. **테스트**: `curl http://localhost:8080/api/health`