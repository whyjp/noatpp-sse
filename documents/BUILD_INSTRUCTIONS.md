# 빌드 가이드

## 시스템 요구사항

### Windows (WSL/네이티브)
```bash
# WSL에서 빌드 도구 설치
sudo apt update
sudo apt install -y cmake build-essential git

# 또는 네이티브 Windows에서 Visual Studio 2022 사용
```

### Linux/Ubuntu
```bash
sudo apt update
sudo apt install -y cmake build-essential git
```

### macOS
```bash
# Xcode 명령행 도구 설치
xcode-select --install

# Homebrew를 통한 cmake 설치
brew install cmake
```

## 빌드 과정

### 1. 의존성 다운로드
```bash
# Linux/macOS
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# Windows
setup_dependencies.bat
```

### 2. 프로젝트 빌드

#### Linux/macOS
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

#### Windows (Visual Studio)
```batch
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### 3. 서버 실행
```bash
# Linux/macOS
./noatpp-sse

# Windows
Release\noatpp-sse.exe
```

## 엔드포인트 테스트

서버가 포트 8080에서 실행 중일 때:

```bash
# 루트 엔드포인트 테스트
curl http://localhost:8080/

# 헬스 체크 엔드포인트 테스트
curl http://localhost:8080/api/health
```

예상 응답:
- `/` 반환: "Oatpp SSE Server is running!"
- `/api/health` 반환: 상태, 메시지, 타임스탬프가 포함된 JSON

## 문제 해결

1. **CMake를 찾을 수 없음**: cmake 패키지 설치
2. **컴파일러 오류**: C++20 호환 컴파일러 확인 (GCC 10+, Clang 12+, MSVC 2019+)
3. **네트워크 문제**: 포트 8080 사용 가능 여부 확인
4. **권한 거부**: 적절한 권한으로 실행 또는 main.cpp에서 포트 변경