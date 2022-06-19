#include <gtest/gtest.h>

#include <XE.h>

TEST( StringTest, TestString )
{
	const char * cstr = "abcdef0123456789智慧中国";
	const wchar_t * wstr = L"abcdef0123456789智慧中国";
	const char * c8str = reinterpret_cast< const char * >( u8"abcdef0123456789智慧中国" );

	XE::AnsiString ansi = "abcdef0123456789智慧中国";
	XE::WideString wide = ansi;
	XE::String utf8 = ansi;

	EXPECT_TRUE( ansi == "abcdef0123456789智慧中国" );

	EXPECT_TRUE( wide == ansi );
	EXPECT_TRUE( ansi == wide );
	EXPECT_TRUE( ansi == utf8 );
	EXPECT_TRUE( utf8 == ansi );
	EXPECT_TRUE( wide == utf8 );
	EXPECT_TRUE( utf8 == wide );


	XE::String str1 = "abcdef0123456789智慧中国";
	XE::String str2 = XE::Format( "abcdef0123{%0}6789智{%1}国", "45", "慧中" );
	XE::String str3 = XE::Format( "abcdef0123{ %0}6789智{%1 }国", "45", "慧中" );
	XE::String str4 = XE::Format( "abcdef0123{  %0}6789智{%1  }国", "45", "慧中" );
	XE::String str5 = XE::Format( "abcdef0123{ %0 }6789智{ %1 }国", "45", "慧中" );
	XE::String str6 = XE::Format( "abcdef0123{  %0  }6789智{  %1  }国", "45", "慧中" );

	EXPECT_TRUE( str1 == str2 );
	EXPECT_TRUE( str1 == str3 );
	EXPECT_TRUE( str1 == str4 );
	EXPECT_TRUE( str1 == str5 );
	EXPECT_TRUE( str1 == str6 );
}
