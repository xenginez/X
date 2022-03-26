#include <gtest/gtest.h>

#include <XE.h>

namespace ReflectTestModule
{
	IMPLEMENT_META_MODULE( ReflectTestModule );

	enum class ReflectTestEnum
	{
		NONE,
		NONE1,
	};
	DECL_META_ENUM( ReflectTestModule, ReflectTestEnum );

	class ReflectTestClass
	{
	public:
		int i = 0;
		float f = 2.0f;
	};
	DECL_META_CLASS( ReflectTestModule, ReflectTestClass );
}

BEG_META( ReflectTestModule::ReflectTestEnum )
type->Value( "NONE", ReflectTestModule::ReflectTestEnum::NONE );
type->Value( "NONE1", ReflectTestModule::ReflectTestEnum::NONE1 );
END_META()

BEG_META( ReflectTestModule::ReflectTestClass )
type->Property( "i", &ReflectTestModule::ReflectTestClass::i );
type->Property( "f", &ReflectTestModule::ReflectTestClass::f );
END_META()



TEST( ReflectTest, TestReflect )
{
	EXPECT_TRUE( sizeof( XE::Variant ) == 24 );
	EXPECT_TRUE( TypeID< XE::uint32 >::Get() == ClassID< XE::uint32 >::Get() );
	EXPECT_TRUE( XE::Reflection::FindClass( "uint32" ) == ClassID< XE::uint32 >::Get() );
	EXPECT_TRUE( XE::Reflection::FindEnum( "ReflectTestModule.ReflectTestEnum" ) == EnumID< ReflectTestModule::ReflectTestEnum >::Get() );
	EXPECT_TRUE( XE::Reflection::FindEnum( "ReflectTestModule.ReflectTestEnum" )->FindValue("NONE").Value< ReflectTestModule::ReflectTestEnum >() == ReflectTestModule::ReflectTestEnum::NONE );

	ReflectTestModule::ReflectTestClass test_class;

	EXPECT_TRUE( XE::Reflection::FindClass( "ReflectTestModule.ReflectTestClass" ) == ClassID< ReflectTestModule::ReflectTestClass >::Get() );

	EXPECT_TRUE( XE::Reflection::FindClass( "ReflectTestModule.ReflectTestClass" )->FindProperty( "i" )->Get( &test_class ).ToInt32() == 0 );
	EXPECT_TRUE( ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "i" )->Get( &test_class ).ToInt32() == 0 );

	EXPECT_TRUE( XE::Reflection::FindClass( "ReflectTestModule.ReflectTestClass" )->FindProperty( "f" )->Get( &test_class ).ToFloat32() == 2.0f );
	EXPECT_TRUE( ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "f" )->Get( &test_class ).ToFloat32() == 2.0f );

	test_class.f = 3.0f;
	test_class.i = 1;

	EXPECT_TRUE( XE::Reflection::FindClass( "ReflectTestModule.ReflectTestClass" )->FindProperty( "i" )->Get( &test_class ).ToInt32() == 1 );
	EXPECT_TRUE( ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "i" )->Get( &test_class ).ToInt32() == 1 );

	EXPECT_TRUE( XE::Reflection::FindClass( "ReflectTestModule.ReflectTestClass" )->FindProperty( "f" )->Get( &test_class ).ToFloat32() == 3.0f );
	EXPECT_TRUE( ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "f" )->Get( &test_class ).ToFloat32() == 3.0f );

	ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "i" )->Set( &test_class, 4 );
	ClassID< ReflectTestModule::ReflectTestClass >::Get()->FindProperty( "f" )->Set( &test_class, 5.0f );

	EXPECT_TRUE( test_class.i == 4 );
	EXPECT_TRUE( test_class.f == 5.0f );
}
