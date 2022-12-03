/*!
 * \file	InputState.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INPUTSTATE_H__76815B9C_CDE1_4EDF_BD99_8E0F97D479A7
#define INPUTSTATE_H__76815B9C_CDE1_4EDF_BD99_8E0F97D479A7

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API InputState : public XE::EnableSharedFromThis< InputState >
{
public:
	InputState();

	~InputState();

public:
	XE::Variant GetVariant( const XE::String & val ) const;

	bool GetBool( const XE::String & val, bool def = false ) const;

	XE::float32 GetFloat( const XE::String & val, XE::float32 def = 0.0f ) const;

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > XE::Variant GetVariant( T val ) const
	{
		return GetVariant( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > bool GetBool( T val, bool def = false ) const
	{
		return GetBool( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ), def );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > XE::float32 GetFloat( T val, XE::float32 def = 0.0f ) const
	{
		return GetFloat( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ), def );
	}

	XE::BasicMemoryView< XE::CodePoint > GetUnicodes() const;

public:
	void PutUnicode( XE::CodePoint val );

	void PutVariant( const XE::String & name, const XE::Variant & val );

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > void PutVariant( T name, const XE::Variant & val )
	{
		PutVariant( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ), val );
	}

public:
	void Clear();

	void ClearChanges();

	void Merge( const XE::InputState & val );

	const XE::Map< XE::String, XE::Variant > & GetChanges() const;

private:
	XE::Array< XE::CodePoint > _Unicodes;
	XE::Map< XE::String, XE::Variant > _Values;
	XE::Map< XE::String, XE::Variant > _Changes;
};

END_XE_NAMESPACE

#endif//INPUTSTATE_H__76815B9C_CDE1_4EDF_BD99_8E0F97D479A7
