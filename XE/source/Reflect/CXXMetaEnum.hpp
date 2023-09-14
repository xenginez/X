/*!
 * \file   CXXMetaEnum.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CXXMETAENUM_HPP__1D036321_0050_4C40_8B6D_4EDFD9A417F4
#define __CXXMETAENUM_HPP__1D036321_0050_4C40_8B6D_4EDFD9A417F4

#include "MetaEnum.h"

BEG_XE_NAMESPACE

template< typename T > class CXXMetaEnum : public XE::MetaEnum
{
public:
	using EnumType = T;

public:
	CXXMetaEnum( const XE::String & Name, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaEnum( Name, sizeof( EnumType ), Owner, Module )
	{

	}

public:
	void Serialize( XE::OArchive & arc, const XE::Variant & val ) const override
	{
		XE::Serializable< EnumType >::Serialize( arc, val.Value< const EnumType & >() );
	}

	void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const override
	{
		XE::Serializable< EnumType >::Deserialize( arc, obj.Value< EnumType & >() );
	}

public:
	void Value( const XE::String & Name, EnumType val )
	{
		_RegisterValue( Name, val );
	}
};

END_XE_NAMESPACE

#define DECL_META_ENUM( _MODULE, _ENUM ) \
template< > struct ::EnumID< _MODULE::_ENUM > \
{ \
	static XE::MetaEnumCPtr Get( const _MODULE::_ENUM * val = nullptr ) \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaEnum< _MODULE::_ENUM > >( XE::Hash( __xe__sig__ ), #_ENUM, nullptr, _MODULE##::GetModule() ); \
		return meta; \
	} \
};

#define DECL_XE_ENUM( _ENUM ) DECL_META_ENUM( XE, _ENUM )

#endif // __CXXMETAENUM_HPP__1D036321_0050_4C40_8B6D_4EDFD9A417F4
