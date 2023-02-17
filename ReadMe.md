# YSUtility

**다양한 기능들을 제공해주는 라이브러리**

## 현재 제공하는 기능들

- enable_shared_from_base
- PassKey

## 사용 방법

ysUtility.hpp 파일을 받아서 사용 또는 CMake 프로젝트에 subdirectory로 복제한 프로젝트를 등록

## CMake프로젝트에서 서브디렉토리로 설정 방법

### 요구 사항

- C++20 지원 컴파일러
- CMake (3.25버전 이상)

### 프로젝트 생성 단계
    
YSUtility 저장소를 복제한다.  

```git
git clone git@github.com:chldbstj4536/YSUtility/git
```

추가할 CMake프로젝트에 target_name 변수설정을 한다

```cmake
set(target_name 적용할_타겟_이름)
```

subdirectory로 복제한 폴더를 추가한다. (add_subdirectory 전에 target 설정이 되어있어야 한다.)

```cmake
add_subdirectory(YSUtility경로)
```

cmake 실행
```powershell
cmake -S . -B build
```

build 폴더에 사용하는 빌드도구에 맞게 프로젝트 생성