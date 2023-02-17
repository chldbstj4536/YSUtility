/**
 * @file ysUtility.h
 * @author 최윤서 (chldbstj4536@gmail.com)
 * @brief 각종 유틸리티 기능들을 모아놓은 헤더파일
 * @version 1.0
 * @date 2023-01-31
 */
#pragma once

#include <memory>
#include <concepts>

#define _YS_BEGIN namespace YS {
#define _YS_END }

_YS_BEGIN
/**
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

#define ENABLE_MAKE_SHARED_DECL private: struct enable_make_shared
#define ENABLE_MAKE_SHARED(_CLASS) \
struct _CLASS::enable_make_shared : public _CLASS\
{\
public:\
    template<typename... _Args>\
    enable_make_shared(_Args&&... args) : _CLASS(std::forward<_Args>(args)...) { }\
}

/**
 * @brief PassKey 패턴에서 어떤 클래스를 통과시킬지 (허용할지) 결정하는 클래스
 * 
 * 사용하고자 하는 멤버함수에 PassKey<_Certs...> 클래스를 파라미터로 두어
 * 가변인자(_Certs...)에 해당하는 클래스만 호출할 수 있도록 구현.
 * 
 * @example
 * 허용하고자 하는 클래스들을 가변 인자 템플릿에 넣어준다.
 * Ret SomeSecureMemFn(PassKey<Foo, Bar, ...>, ...);
 * 
 * @tparam _Certs 호출 가능한 클래스들
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
     * @tparam _Cert 통과하고자 하는 클래스의 타입
     */
    template <class _Cert>
    requires (same_as<_Certs, _Cert> || ...)
    PassKey(PassKey<_Cert> const &) {}

    PassKey(PassKey const&) = delete;
    PassKey(PassKey &&) = delete;
    ~PassKey() = default;
    PassKey& operator=(PassKey const&) = delete;
    PassKey& operator=(PassKey &&) = delete;
};
/**
 * @brief PassKey 패턴에서 자신을 인증하는데 사용되는 CRTP 클래스
 * 
 * PassKey 패턴을 템플릿을 사용하여 구현
 * 
 * PassKey<Self>를 상속받아 인증을 요구하는(PassKey<_Certs...>를 입력받는) 함수에서 인증하는데 사용
 * 
 * @example
 * 클래스 상속 후
 * class Foo : public PassKey<Foo> {}
 * 호출하고자 하는 함수의 인증 매개변수에 자기 자신을 대입
 * SomeSecureMemFn(*this, ...);
 * 
 * 또는 전역 멤버 함수의 경우 PassKey를 생성 후 인증
 * static Foo::StaticMemFn()
 * {
 *     static PassKey<Foo> key;
 *     SomeSecureFn(key, ...);
 * }
 * 
 * @tparam _Cert 인증하고자 하는 클래스
 */
template <class _Cert>
class PassKey<_Cert>
{
private:
    PassKey() = default;

    friend _Cert;
};
_YS_END