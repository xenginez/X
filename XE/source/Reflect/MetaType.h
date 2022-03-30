/*!
 * \file   MetaType.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/12
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6
#define __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6

#include <functional>

#include "MetaInfo.h"

BEG_XE_NAMESPACE

class XE_API MetaType : public MetaInfo
{
public:
	MetaType( const String& Name, MetaInfoType Type, XE::uint64 Size, MetaInfoCPtr Owner, MetaModuleCPtr Module );

	~MetaType();

public:
	XE::uint64 GetSize() const;

public:
	const XE::Array< XE::MetaAttributeCPtr > & GetAttributes() const;

	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & val ) const;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( ClassID< T >::Get() ) );
	}

public:
	virtual void Serialize( XE::OArchive & arc, const XE::Variant & obj ) const = 0;

	virtual void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const = 0;

public:
	template< typename T > MetaType & Attribute( const T & val )
	{
		static_assert( std::is_base_of_v< XE::MetaAttribute, XE::TypeTraits< T >::raw_t >, "does not belong to meta attribute" );

		for ( auto & attr : _Attributes )
		{
			XE_ASSERT( ::TypeID< T >::Get() != attr->GetMetaClass() && "" );
		}

		_Attributes.push_back( XE::MakeShared< T >( val ) );

		return *this;
	}

	template< typename T, typename ... ARGS > MetaType & Attribute( const T & val, ARGS &&... args )
	{
		Attribute( val );

		Attribute( std::forward< ARGS >( args )... );

		return *this;
	}

private:
	XE::uint64 _Size;
	XE::Array< XE::MetaAttributeCPtr > _Attributes;
};

END_XE_NAMESPACE

#endif // __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6
