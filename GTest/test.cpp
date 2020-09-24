#include "pch.h"

#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Data.h>

using namespace winrt::MyLibrary;
using namespace std::chrono;
using namespace std::chrono_literals;
using namespace winrt::Windows::Foundation;


TEST(ClassTests, Ctor) {
	Class c = Class();
	EXPECT_EQ(0, c.MyProperty());
}

/*

	Google test framework does not support async directly but one can still run
	the tests by running code synchronously
*/

TEST(AsyncTests, RunAsync)
{
	Class::WasteTimeAsync(500ms).get();
}

/*
	.get() does not have a timeout
*/

template<typename Async> void TestAsyncCode(Async action, winrt::Windows::Foundation::TimeSpan timeout = 30s)
{
	winrt::handle waitEvent{ ::CreateEventEx(nullptr,nullptr,0,EVENT_ALL_ACCESS) };
	HANDLE waitHandle = waitEvent.get();

	auto asyncAction = action();
	asyncAction.Completed(
		{
			[=](IAsyncAction const& a, AsyncStatus const& status) {
				::SetEvent(waitHandle);
			}
		});

	::WaitForSingleObject(
		waitEvent.get(),
		(DWORD)duration_cast<milliseconds, TimeSpan::rep, TimeSpan::period>(timeout).count()
	);

	if (asyncAction.Status() == AsyncStatus::Started) {
		throw std::exception("Timeout occured");
	}
	else if (asyncAction.Status() == AsyncStatus::Error)
	{
		throw winrt::hresult_error(asyncAction.ErrorCode());
	}
}

TEST(AsyncTests, RunAsyncWithTimeout)
{
	TestAsyncCode(
		[] () -> winrt::Windows::Foundation::IAsyncAction 
		{
			co_await Class::WasteTimeAsync(500ms);
		});
}

TEST(AsyncTests, RunAsyncThatWillTimeout)
{
	TestAsyncCode(
		[]() -> winrt::Windows::Foundation::IAsyncAction
		{
			co_await Class::WasteTimeAsync(15s);
		}, 1s);
}

class UITests : public ::testing::Test
{
protected:
	static winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager g_xaml;
	static void SetUpTestCase()
	{
		g_xaml = winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager::InitializeForCurrentThread();
	}
	static void TearDownTestCase()
	{
		g_xaml.Close();
	}
};

winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager UITests::g_xaml{ nullptr };

TEST_F(UITests, PropertyChangedEventArgs)
{
	/* This would fail with RPC_E_WRONG_THREAD if XAML is not initialized */
	auto args = winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs(L"Name");
}