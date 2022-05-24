/*!
 * \file   InputService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/08
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __INPUTSERVICE_H__1AA680A2_3C45_4FAF_A62A_EF525EE5FFFD
#define __INPUTSERVICE_H__1AA680A2_3C45_4FAF_A62A_EF525EE5FFFD

#include "Type.h"

#include "Core/Service.h"

BEG_XE_NAMESPACE

class XE_API InputService : public XE::Service
{
	OBJECT( InputService, Service )

private:
	struct Private;

public:
	InputService();

	~InputService() override;

public:
	void Prepare() override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	bool GetButton( const XE::String & val ) const;

	bool GetButtonUp( const XE::String & val ) const;

	bool GetButtonDown( const XE::String & val ) const;

	XE::float32 GetAxis( const XE::String & val ) const;

	XE::Variant GetValue( const XE::String & val ) const;

	XE::BasicMemoryView< XE::CodePoint > GetUnicodes() const;

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > bool GetButton( T val ) const
	{
		return GetButton( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > bool GetButtonUp( T val ) const
	{
		return GetButtonUp( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > bool GetButtonDown( T val ) const
	{
		return GetButtonDown( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > XE::float32 GetAxis( T val ) const
	{
		return GetAxis( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

	template< typename T, std::enable_if_t< std::is_enum_v< T >, int > = 0 > XE::Variant GetValue( T val ) const
	{
		return GetValue( ::EnumID< T >::Get()->GetFullName() + "." + ::EnumID< T >::Get()->FindName( val ) );
	}

public:
	void Vibrate( XE::float32 left, XE::float32 right, XE::uint32 index = 0 );

public:
	void ClearStatus();

	const XE::Map< XE::String, XE::Variant > & GetChangeStatus() const;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __INPUTSERVICE_H__1AA680A2_3C45_4FAF_A62A_EF525EE5FFFD
