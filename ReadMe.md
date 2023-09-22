# YSUtility

**다양한 기능들을 제공해주는 라이브러리**

## 현재 제공하는 기능들

- enable_shared_from_base
- PassKey

## 사용 방법

ysUtility.hpp 파일을 받아서 사용 또는 CMake 프로젝트에서 CMakeLists.txt에 명령어를 추가해 설치 후 사용

## 요구 사항

- C++20 지원 컴파일러
- CMake (3.25버전 이상)

## CMake프로젝트에서 사용방법

YSUtility 저장소를 복제한다.  

```git
git clone --recurse-submodules https://github.com/chldbstj4536/YSUtility.git
```

아래 명령어를 통하여 YSUtility 프로젝트를 설치한다.

```cmake
execute_process(COMMAND ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_SOURCE_DIR}/submodules/YSDefine -B ${CMAKE_CURRENT_BINARY_DIR}/YSDefine -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})
execute_process(COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/YSDefine --target install)
```
- CMAKE_CURRENT_SOURCE_DIR: 현재 프로젝트의 CMakeLists.txt의 폴더 위치
- CMAKE_CURRENT_BINARY_DIR: 현재 프로젝트의 바이너리(빌드 파일들이 저장될) 폴더 위치
- CMAKE_INSTALL_PREFIX: 실치할 폴더 위치(inc, lib 처럼 빌드 결과물 또는 헤더 파일 등)

cmake 실행

```powershell
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="./out"
```
- -S {Param}: CMakeLists.txt의 위치
- -B {Param}: Binary 폴더 위치
- -DCMAKE_INSTALL_PREFIX="{Param}": 설치 폴더 경로 설정