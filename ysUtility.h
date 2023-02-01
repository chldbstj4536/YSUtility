#pragma once
/**
 * @file ysUtility.h
 * @author 최윤서 (chldbstj4536@gmail.com)
 * @brief 각종 유틸리티 기능들을 모아놓은 헤더파일
 * @version 1.0
 * @date 2023-01-31
 */

#include <memory>
#include <concepts>

#define _YS_BEGIN namespace YS {
#define _YS_END }

_YS_BEGIN
/**
 * @brief std::enable_shared_from_this를 상속관계에서도 편하게 사용하기 위한 helpper 클래스
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
     * 잘못된 타입 입력 시 nullptr 반환
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
     * 잘못된 타입 입력 시 nullptr 반환
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
#define ENABLE_MAKE_SHARED_DECL private: struct enable_make_shared;
#define ENABLE_MAKE_SHARED(_CLASS) \
struct _CLASS::enable_make_shared : public _CLASS\
{\
public:\
    template<typename... _Args>\
    enable_make_shared(_Args&&... args) : _CLASS(std::forward<_Args>(args)...) { }\
};
_YS_END