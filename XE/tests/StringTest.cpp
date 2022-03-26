#include <gtest/gtest.h>

#include <XE.h>

TEST( StringTest, TestString )
{
	const char * cstr = "abcdef0123456789�ǻ��й�";
	const wchar_t * wstr = L"abcdef0123456789�ǻ��й�";
	const char * c8str = u8"abcdef0123456789�ǻ��й�";

	XE::AnsiString ansi = "abcdef0123456789�ǻ��й�";
	XE::WideString wide = ansi;
	XE::Utf8String utf8 = ansi;

	EXPECT_TRUE( ansi == "abcdef0123456789�ǻ��й�" );

	EXPECT_TRUE( wide == ansi );
	EXPECT_TRUE( ansi == wide );
	EXPECT_TRUE( ansi == utf8 );
	EXPECT_TRUE( utf8 == ansi );
	EXPECT_TRUE( wide == utf8 );
	EXPECT_TRUE( utf8 == wide );


	XE::String str1 = "abcdef0123456789�ǻ��й�";
	XE::String str2 = XE::Format( "abcdef0123{%0}6789��{%1}��", "45", "����" );
	XE::String str3 = XE::Format( "abcdef0123{ %0}6789��{%1 }��", "45", "����" );
	XE::String str4 = XE::Format( "abcdef0123{  %0}6789��{%1  }��", "45", "����" );
	XE::String str5 = XE::Format( "abcdef0123{ %0 }6789��{ %1 }��", "45", "����" );
	XE::String str6 = XE::Format( "abcdef0123{  %0  }6789��{  %1  }��", "45", "����" );

	EXPECT_TRUE( str1 == str2 );
	EXPECT_TRUE( str1 == str3 );
	EXPECT_TRUE( str1 == str4 );
	EXPECT_TRUE( str1 == str5 );
	EXPECT_TRUE( str1 == str6 );
}
