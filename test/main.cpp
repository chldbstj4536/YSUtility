#include <ysUtility.hpp>
#include <iostream>
#include <format>

#pragma region enable_shared_from_base Test
namespace enable_shared_from_base
{
    class foo : public YS::enable_shared_from_base<foo>
    {
    public:
        foo() = default;
        virtual ~foo() = default;

        template <std::derived_from<foo> _Derived>
        std::shared_ptr<_Derived> GetSharedPtr()
        {
            return shared_from_base<_Derived>();
        }
    };
    class bar : public foo
    {
    public:
        bar() = default;
        virtual ~bar() = default;
    };
    class baz : public foo
    {
    public:
        baz() = default;
        virtual ~baz() = default;
    };
}
#pragma endregion

#pragma region PassKey Test
namespace PassKey
{
    class foo;
    class bar;

    // foobar의 SomeSecureFn함수는 Passkey<foo, bar>를 요구하므로
    // foo 또는 bar만 접근할 수 있다.
    class foobar
    {
    public:
        void SomeSecureFn(YS::PassKey<foo, bar>) { }
    };

    // 상속을 통한 사용법
    // 장점 : PassKey를 생성하는데 추가적인 비용이 없다.
    // 단점 : 이미 객체가 생성되어 있어야 사용할 수 있다.
    class foo : public YS::PassKey<foo>
    {
    public:
        void CallSomeSecureFn(foobar& fb)
        {
            fb.SomeSecureFn(*this);
        }
    };

    // 전역 객체 생성을 통한 사용법
    // 장점 : 객체가 먼저 생성되어 있지 않아도 사용할 수 있다.
    // 단점 : PassKey를 생성하는데 1바이트의 비용이 발생한다.
    class bar
    {
    public:
        // 전역 멤버 함수이기 때문에 this포인터를 사용할 수 없다
        // 이 경우 전역 PassKey 객체를 생성하여 사용할 수 있다.
        static void CallSomeSecureFn(foobar& fb)
        {
            static YS::PassKey<bar> key;
            fb.SomeSecureFn(key);
        }
    };

    class baz : public YS::PassKey<baz>
    {
    public:
        void CallSomeSecureFn(foobar& fb)
        {
            // PassKey<baz>는 허용되지 않기 때문에 에러 발생
            //fb.SomeSecureFn(*this);
        }
    };

    // 실용적인 예로 싱글톤 패턴을 들 수 있다.
    // 싱글톤 객체은 단 하나의 전역 객체만 존재해야 하기 때문에 생성자는 아무나 호출할 수 없다.
    // 생성자를 private으로 하는 방법도 있지만, 만일 make_shared를 통해 생성하고자 한다면 접근범위 제한으로 인해 문제가 생긴다.
    // PassKey를 사용한다면 이러한 문제를 해결할 수 있다.
    template <typename T>
    class Singleton
    {
    public:
        // public 생성자이지만 PassKey<Singletone>을 생성할 수 있는 방법은 오직 Singleton 클래스이기 때문에 외부에서 생성이 불가능하다.
        Singleton(YS::PassKey<Singleton>) {}

        static std::shared_ptr<Singleton> GetInstance()
        {
            static T instance = std::make_shared<T>(YS::PassKey<Singleton>());
            return instance;
        }
    };
}
#pragma endregion

int main()
{
    // enable_shared_from_base Test
    {
        using namespace enable_shared_from_base;
        std::cout << "============= enable_shared_from_base Test =============\n";
        auto pBar = std::make_shared<bar>();
        std::cout << std::format("pBar's use_count : {}\n", pBar.use_count());
        {
            auto getBar = pBar->GetSharedPtr<bar>();
            std::cout << std::format("pBar's use_count : {}\n", pBar.use_count());
            std::cout << std::format("getBar's use_count : {}\n", getBar.use_count());
            auto getBazFromBar = pBar->GetSharedPtr<baz>();
            std::cout << std::format("pBar's use_count : {}\n", pBar.use_count());
            std::cout << std::format("getBazFromBar's use_count : {}\n", getBazFromBar.use_count());
        }
        std::cout << std::format("pBar's use_count : {}\n\n\n", pBar.use_count());
    }

    // PassKey Test
    {
        using namespace PassKey;
        std::cout << "============= PassKey Test =============\n";
        foobar fb;
        foo f;
        baz bz;

        f.CallSomeSecureFn(fb);
        bar::CallSomeSecureFn(fb);
        bz.CallSomeSecureFn(fb);
        std::cout << "\n\n";
    }

    return 0;
}