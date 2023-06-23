/**
 * @file ysUtility.h
 * @author 최윤서 (chldbstj4536@gmail.com)
 * @brief 다양한 유틸리티 기능들을 모아놓은 헤더파일
 * @version 1.0.0
 * @date 2023-02-17
 */
#pragma once

#include <memory>
#include <concepts>
#include <random>

#define _YS_BEGIN namespace YS {
#define _YS_END }

_YS_BEGIN
/**
 * @class enable_shared_from_base
 * @brief std::enable_shared_from_this를 상속관계에서도 편하게 사용하기 위한 helper 클래스
 */
template<class _Base>
class enable_shared_from_base : public std::enable_shared_from_this<_Base>
{
protected:
    using std::enable_shared_from_this<_Base>::shared_from_this;
    using std::enable_shared_from_this<_Base>::weak_from_this;

    /**
     * @brief shared_from_this()를 파생클래스로 형변환 해주는 함수
     * 
     * @tparam _Derived 변환할 파생클래스 타입
     * @return std::shared_ptr<_Derived>
     * @retval nullptr 잘못된 형변환 시 nullptr 반환
     */
    template <std::derived_from<_Base> _Derived>
    std::shared_ptr<_Derived> shared_from_base()
    {
        return std::dynamic_pointer_cast<_Derived>(shared_from_this());
    }
    /**
     * @brief weak_from_this()를 파생클래스로 형변환 해주는 함수
     * 
     * @tparam _Derived 변환할 파생클래스 타입
     * @return std::weak_ptr<_Derived>
     * @retval nullptr 잘못된 형변환 시 nullptr 반환
     */
    template <std::derived_from<_Base> _Derived>
    std::weak_ptr<_Derived> weak_from_base()
    {
        return std::dynamic_pointer_cast<_Derived>(weak_from_this());
    }
};

/**
 * @class PassKey
 * @brief PassKey 패턴에서 어떤 클래스를 통과시킬지 (허용할지) 결정하는 클래스
 * 
 * 사용하고자 하는 멤버함수에 PassKey<_Certs...> 클래스를 파라미터로 두어
 * 가변인자(_Certs...)에 해당하는 클래스만 호출할 수 있도록 구현.
 * 
 * @tparam _Certs 호출 가능한 클래스들
 * 
 * @see 인증키로 사용되는 PassKey<_Cert> 클래스 참고
 * 
 * @example passkey_example.cpp
 * 이 예제는 PassKey를 어떻게 사용하는지에 대한 예제입니다.
 */
template<class... _Certs>
class PassKey final
{
public:
    /**
     * @brief 접근 제한 생성자
     * 
     * 템플릿 생성자로 _Cert 타입 매개변수를 맏는데 해당 타입이 _Certs에 있는지에 대한 제약조건이 붙어있다.
     * 따라서 해당 제약조건을 만족하지 못하면 템플릿은 작동하지 못하고 생성자를 호출할 수 없게 되면서 접근 제한 역할을 한다.
     * 
     * @tparam _Cert 제약조건을 만족하는지 검사할 타입
     */
    template <class _Cert>
    requires (std::same_as<_Certs, _Cert> || ...)
    PassKey(PassKey<_Cert> const &) {}

    PassKey(PassKey const&) = delete;
    PassKey(PassKey &&) = delete;
    ~PassKey() = default;
    PassKey& operator=(PassKey const&) = delete;
    PassKey& operator=(PassKey &&) = delete;
};
/**
 * @class PassKey<_Cert>
 * @brief PassKey 패턴에서 자신을 인증하는데 사용되는 CRTP 클래스
 * 
 * 인증을 요구하는(PassKey<_Certs...>를 입력받는) 함수에서 인증하는데 사용
 * 자세한 사용법은 @ref passkey_example.cpp 참고
 * 
 * @tparam _Cert 인증하고자 하는 클래스
 * 
 * @see 인증을 요구하는데 사용되는 PassKey 클래스 참고
 * 
 * @example passkey_example.cpp
 * 이 예제는 PassKey를 어떻게 사용하는지에 대한 예제입니다.
 */
template <class _Cert>
class PassKey<_Cert>
{
private:
    /**
     * @brief _Cert만 PassKey<_Cert>를 생성할 수 있도록 private으로 제한
     */
    PassKey() = default;

    friend _Cert;
};

/**
 * @brief 난수 생성기 얻기
 * 
 * mt19937 메르센 트위스터라는 알고리즘 사용
 * rand의 선형합동방식보다 상관관계가 더 작아서 양질의 난수값 제공
 * 단 객체의 크기가 커서 (2KB 이상) 메모리가 부족한 시스템에서는 minstd_rand가 적합할 수 있다.
 * random_device 운영체제 레벨에서 제공하는 진짜 난수를 사용
 * 단, 의사 난수보다 난수를 생성하는 속도가 매우 느리다.
 * 따라서 시드값처럼 난수 엔진을 생성하는데 사용하고, 그 이후의 난수열은 난수 엔진으로 생성.
 * 
 * @return const std::mt19937& 난수 생성기
 */
const std::mt19937& GetGenerator()
{
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

/**
 * @brief 난수를 생성하는 함수
 * 
 * @param min 최소 범위
 * @param max 최대 범위
 * @return float [min, max]사이의 무작위 수
 */
float Random(float min, float max)
{
    std::uniform_real_distribution<float> dis(min, max);
    return dis(GetGenerator());
}

/**
 * @brief 난수를 생성하는 함수
 * 
 * @param min 최소 범위
 * @param max 최대 범위
 * @return int [min, max]사이의 무작위 수
 */
int Random(int min, int max)
{
    std::uniform_int_distribution<int> dis(min, max);
    return dis(GetGenerator());
}
_YS_END


/*!
@mainpage ReadMe

# YSUtility

__다양한 기능들을 제공해주는 라이브러리__

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
 */