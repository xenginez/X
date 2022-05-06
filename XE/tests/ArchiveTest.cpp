#include <gtest/gtest.h>

#include <XE.h>


namespace ArchiveTestModule
{
	IMPLEMENT_META_MODULE( ArchiveTestModule );

	class ArchiveTestClass
	{
	public: 
		int i = 0;
		float f = 1.0f;
		XE::String s = "abcdef1234567890智慧中国";
	};
}

DECL_META_CLASS( ArchiveTestModule, ArchiveTestClass );


BEG_META( ArchiveTestModule::ArchiveTestClass )
type->Property( "i", &ArchiveTestModule::ArchiveTestClass::i );
type->Property( "f", &ArchiveTestModule::ArchiveTestClass::f );
type->Property( "s", &ArchiveTestModule::ArchiveTestClass::s );
END_META()


TEST( ArchiveTest, TestArchive )
{
	{
		ArchiveTestModule::ArchiveTestClass test_class;

		std::stringstream stream;

		{
			XE::XmlOArchive oa( stream );
			oa << test_class;
		}

		test_class.i = 1;
		test_class.f = 2.0f;
		test_class.s = "gjfioaghfuidty483tkd";

		{
			XE::XmlIArchive ia( stream );
			ia >> test_class;
		}

		EXPECT_TRUE( test_class.i == 0 );
		EXPECT_TRUE( test_class.f == 1.0f );
		EXPECT_TRUE( test_class.s == "abcdef1234567890智慧中国" );
	}
	{
		ArchiveTestModule::ArchiveTestClass test_class;

		std::stringstream stream;

		{
			XE::JsonOArchive oa( stream );
			oa << test_class;
		}

		test_class.i = 1;
		test_class.f = 2.0f;
		test_class.s = "gjfioaghfuidty483tkd";

		{
			XE::JsonIArchive ia( stream );
			ia >> test_class;
		}

		EXPECT_TRUE( test_class.i == 0 );
		EXPECT_TRUE( test_class.f == 1.0f );
		EXPECT_TRUE( test_class.s == "abcdef1234567890智慧中国" );
	}
	{
		ArchiveTestModule::ArchiveTestClass test_class;

		XE::MemoryStream stream;

		{
			XE::BinaryOArchive oa( stream );
			oa << test_class;
		}

		test_class.i = 1;
		test_class.f = 2.0f;
		test_class.s = "gjfioaghfuidty483tkd";

		{
			XE::BinaryIArchive ia( stream );
			ia >> test_class;
		}

		EXPECT_TRUE( test_class.i == 0 );
		EXPECT_TRUE( test_class.f == 1.0f );
		EXPECT_TRUE( test_class.s == "abcdef1234567890智慧中国" );
	}


	{
		XE::EventPtr event = XE::MakeShared< XE::Event >( XE::HandleCast< XE::Event >( 2 ), XE::uint32( 1 ) );
		std::stringstream stream;

		{
			XE::XmlOArchive oa( stream );
			oa << event;
		}

		event = nullptr;

		{
			XE::XmlIArchive ia( stream );
			ia >> event;
		}

		EXPECT_TRUE( event->handle.GetValue() == 2 );
		EXPECT_TRUE( event->parameter.ToUInt32() == 1 );
	}
	{
		XE::EventPtr event = XE::MakeShared< XE::Event >( XE::HandleCast< XE::Event >( 2 ), XE::uint32( 1 ) );
		std::stringstream stream;

		{
			XE::JsonOArchive oa( stream );
			oa << event;
		}

		event = nullptr;

		{
			XE::JsonIArchive ia( stream );
			ia >> event;
		}

		EXPECT_TRUE( event->handle.GetValue() == 2 );
		EXPECT_TRUE( event->parameter.ToUInt32() == 1 );
	}
	{
		XE::EventPtr event = XE::MakeShared< XE::Event >( XE::HandleCast< XE::Event >( 2 ), XE::uint32( 1 ) );
		XE::MemoryStream stream;

		{
			XE::BinaryOArchive oa( stream );
			oa << event;
		}

		event = nullptr;

		{
			XE::BinaryIArchive ia( stream );
			ia >> event;
		}

		EXPECT_TRUE( event->handle.GetValue() == 2 );
		EXPECT_TRUE( event->parameter.ToUInt32() == 1 );
	}
}