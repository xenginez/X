#include <gtest/gtest.h>

#include <XE.h>

void func1()
{
	std::cout << "void func1()" << std::endl;
}
void func2( int i )
{
	std::cout << "void func2( int i ) " << i << std::endl;
}
int func3( int x, int y )
{
	return x + y;
}

class func_class
{
public:
	int i = 0;

public:
	void add()
	{
		i += 1;
	}
};

TEST( UtilsTest, TestUtils )
{
	{
		std::string in( "abcdef1234567890智慧中国" );
		std::string out, out2;
		out.resize( XE::BASE64::EncodedLength( in.size() ) );
		out2.resize( in.size() );

		XE::BASE64::Encode( in.c_str(), in.size(), out.data(), out.size() );
		XE::BASE64::Decode( out.c_str(), out.size(), out2.data(), out2.size() );

		EXPECT_TRUE( in == out2 );
	}
	{
		XE::UUID uuid2, uuid1 = XE::UUID::Create();

		XE::String str = uuid1.ToString( "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" );
		uuid2.FromString( str, "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" );

		EXPECT_TRUE( uuid1 == uuid2 );
	}
	{
		std::string str1, str2;

		{
			XE::Zipper zip;
			if( zip.Open( std::filesystem::current_path() / "aa.zip" ) )
			{
				std::stringstream stream;
				stream << "abcdef1234567890智慧中国0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
				
				if( !zip.ExtractEntry( "string" ) )
				{
					zip.AddEntry( "string", stream );
				}

				str1 = stream.str();
			}
		}
		{
			XE::Zipper zip;
			if( zip.Open( std::filesystem::current_path() / "aa.zip" ) )
			{
				std::stringstream stream;
				zip.GetEntry( "string", stream );
				str2 = stream.str();
			}
		}

		std::filesystem::remove( std::filesystem::current_path() / "aa.zip" );
		std::filesystem::remove( std::filesystem::current_path() / "aa0.zip" );

		EXPECT_TRUE( str1 == str2 );
	}
	{
		int i = 0;
		int ii = 0;
		float f = 2.0f;

		XE::Delegate< int() > d0 = []()
		{
			return 5;
		};
		XE::Delegate< void() > d1 = [&i]()
		{
			i = 5;
		};
		XE::Delegate< void() > d2 = func1;
		XE::Delegate< void( int ) > d3 = func2;
		XE::Delegate< int( int, int ) > d4 = func3;

		EXPECT_TRUE( d0() == 5 );
		d1();
		d2();
		d3( i );
		ii = d4( i, f );
		EXPECT_TRUE( ii == 7 );

		XE::SharedPtr<func_class> c = XE::MakeShared<func_class>();

		XE::Delegate<void( func_class * )> d5 = { &func_class::add };
		d5( c.get() );
		EXPECT_TRUE( c->i == 1 );

		d1 = { &func_class::add, c.get() };
		d1();
		EXPECT_TRUE( c->i == 2 );

		int * pi = &c->i;
		d1 = { &func_class::add, c };
		c = nullptr;
		d1();
		EXPECT_TRUE( *pi == 3 );
	}
	{
		int i = 0;
		XE::MakePromise< void >( []( auto res, auto rej )
		{
			std::thread( [res]() mutable { res(); } ).detach();
		} ).Then( []() mutable
		{
			return 7;
		} ).Then( []( int ii ) mutable
		{
			EXPECT_TRUE( ii == 7 );
		} ).Then( [&i]() mutable
		{
			i = 2;
		} );

		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

		EXPECT_TRUE( i == 2 );
	}
}
