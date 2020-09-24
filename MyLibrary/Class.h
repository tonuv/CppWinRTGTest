#pragma once

#include "Class.g.h"

namespace winrt::MyLibrary::implementation
{
    struct Class : ClassT<Class>
    {
        int32_t _value{ 0 };

        Class() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);
        static Windows::Foundation::IAsyncAction WasteTimeAsync(winrt::Windows::Foundation::TimeSpan const &forHowLong);
    };
}

namespace winrt::MyLibrary::factory_implementation
{
    struct Class : ClassT<Class, implementation::Class>
    {
    };
}
