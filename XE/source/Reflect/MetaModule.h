/*!
 * \file	MetaModule.h
 *
 * \author	ZhengYuanQing
 * \date	2020/12/30
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef IMETAMODULE_H__1E0F37E8_64B1_4FE5_B030_88A6326CB205
#define IMETAMODULE_H__1E0F37E8_64B1_4FE5_B030_88A6326CB205

#include <functional>

#include "MetaInfo.h"

BEG_XE_NAMESPACE

class XE_API MetaModule : public XE::MetaInfo
{
	friend class MetaInfo;

public:
	MetaModule( const String & Name, MetaInfoCPtr Owner );

	~MetaModule();

public:
	const XE::Array< XE::MetaInfoCPtr > & GetMetaInfos() const;

public:
	XE::MetaInfoCPtr FindMetaInfo( XE::uint64 hash ) const;

	XE::MetaInfoCPtr FindMetaInfo( const String & FullName ) const;

	void Visit( const XE::Delegate< void( const XE::MetaInfoCPtr & ) > & val ) const;

private:
	void RegisterMetaInfo( MetaInfoPtr val );

private:
	XE::Array< XE::MetaInfoCPtr > _MetaInfos;
};

END_XE_NAMESPACE

#define IMPLEMENT_META_MODULE_1( _MODULE ) \
	static XE::MetaModulePtr GetModule() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static XE::MetaModulePtr meta = XE::MetaInfo::NewMetaInfo< XE::MetaModule >( XE::Hash( __xe__sig__ ), #_MODULE, nullptr ); \
		return meta; \
	}

#define IMPLEMENT_META_MODULE_2( _MODULE, _OWNER ) \
	static XE::MetaModulePtr GetModule() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static XE::MetaModulePtr meta = XE::MetaInfo::NewMetaInfo< XE::MetaModule >( XE::Hash( __xe__sig__ ), #_MODULE, _OWNER::GetModule() ); \
		return meta; \
	}
#define IMPLEMENT_META_MODULE(...)            MACRO_EXP_(MACRO_GLUE(IMPLEMENT_META_MODULE_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

static XE::MetaModulePtr GetModule()
{
	return nullptr;
}

namespace std
{
	IMPLEMENT_META_MODULE( std );

	namespace pmr
	{
		IMPLEMENT_META_MODULE( pmr, std );
	}
}

namespace XE
{
	IMPLEMENT_META_MODULE( XE );
}

#endif // IMETAMODULE_H__1E0F37E8_64B1_4FE5_B030_88A6326CB205
