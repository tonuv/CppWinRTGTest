#include "pch.h"
#include "Class.h"
#include "Class.g.cpp"
#include <chrono>

using namespace std::chrono_literals;

/* 
The following section shows how to load a dll dynamically,
so that it would work both when executed in Win32 process as 
well as UWP app
*/
extern "C"
{
    WINBASEAPI
        _Ret_maybenull_
        HMODULE
        WINAPI
        LoadLibraryW(
            _In_ LPCWSTR lpLibFileName
        );
}

struct hmodule_traits : winrt::handle_traits
{
    using type = HMODULE;

    static void close(type value) noexcept
    {
        FreeLibrary(value);
    }

    static constexpr type invalid() noexcept
    {
        return NULL;
    }
};

using module_handle = winrt::handle_type<hmodule_traits>;

namespace winrt::MyLibrary::implementation
{
    int32_t Class::MyProperty()
    {
        return _value;
    }

    void Class::MyProperty(int32_t value)
    {
        _value = value;
    }

    float Class::GetSquare(float value)
    {
        module_handle hModule { ::LoadLibraryW(L"Dll1.dll") };
        
        auto proc = (float (*)(float)) GetProcAddress(hModule.get(), "GetSquare");

        return proc(value);
    }

    Windows::Foundation::IAsyncAction Class::WasteTimeAsync(winrt::Windows::Foundation::TimeSpan const &forHowLong)
    {

        co_await forHowLong;
    }

}
