#include "pch.h"
#include "Class.h"
#include "Class.g.cpp"
#include <chrono>

using namespace std::chrono_literals;

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
    Windows::Foundation::IAsyncAction Class::WasteTimeAsync(winrt::Windows::Foundation::TimeSpan const &forHowLong)
    {
        co_await forHowLong;
    }

}
