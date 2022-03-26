/*!
 * \file   Reflection.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __REFLECTION_H__7A09BEF6_4373_410D_94CD_7AF277A37AB3
#define __REFLECTION_H__7A09BEF6_4373_410D_94CD_7AF277A37AB3

#include "CXXMetaEnum.hpp"
#include "CXXMetaClass.hpp"

BEG_XE_NAMESPACE

class XE_API Reflection : public XE::NonCopyable
{
	friend class MetaInfo;

private:
	struct Private;

private:
	Reflection();

	~Reflection();

	static Reflection * Instance();

private:
	static void RegisterMetaInfo( XE::MetaInfoPtr val );

public:
	static void VisitMeta( const XE::Delegate< void( const XE::MetaInfoPtr & ) > & val );

	static void VisitType( const XE::Delegate< void( const XE::MetaTypePtr & ) > & val );

	static void VisitEnum( const XE::Delegate< void( const XE::MetaEnumPtr & ) > & val );

	static void VisitClass( const XE::Delegate< void( const XE::MetaClassPtr & ) > & val );

	static void VisitModule( const XE::Delegate< void( const XE::MetaModulePtr & ) > & val );

	static void VisitMethod( const  XE::Delegate< void( const XE::MetaMethodPtr & ) > & val );

	static void VisitProperty( const XE::Delegate< void( const  XE::MetaPropertyPtr & ) > & val );

	static void VisitFunction( const XE::Delegate< void( const  XE::MetaFunctionPtr & ) > & val );

	static void VisitVariable( const XE::Delegate< void( const  XE::MetaVariablePtr & ) > & val );

public:
	static XE::MetaInfoPtr FindMeta( XE::uint64 HashCode );

	static XE::MetaTypePtr FindType( XE::uint64 HashCode );

	static XE::MetaEnumPtr FindEnum( XE::uint64 HashCode );

	static XE::MetaClassPtr FindClass( XE::uint64 HashCode );

	static XE::MetaModulePtr FindModule( XE::uint64 HashCode );

	static XE::MetaMethodPtr FindMethod( XE::uint64 HashCode );

	static XE::MetaPropertyPtr FindProperty( XE::uint64 HashCode );

	static XE::MetaFunctionPtr FindFunction( XE::uint64 HashCode );

	static XE::MetaVariablePtr FindVariable( XE::uint64 HashCode );

public:
	static XE::MetaInfoPtr FindMeta( const XE::String & FullName );

	static XE::MetaTypePtr FindType( const XE::String & FullName );

	static XE::MetaEnumPtr FindEnum( const XE::String & FullName );

	static XE::MetaClassPtr FindClass( const XE::String & FullName );

	static XE::MetaModulePtr FindModule( const XE::String & FullName );

	static XE::MetaMethodPtr FindMethod( const XE::String & FullName );

	static XE::MetaPropertyPtr FindProperty( const XE::String & FullName );

	static XE::MetaFunctionPtr FindFunction( const XE::String & FullName );

	static XE::MetaVariablePtr FindVariable( const XE::String & FullName );

private:
	Private * _p;
};

END_XE_NAMESPACE

#define BEG_META( CLASS ) \
namespace XE \
{ \
	template< > struct MetaTypeCollector< CLASS > \
	{ \
		using ThisType = CLASS; \
		template< typename T > struct Enum \
		{ \
			static XE::SharedPtr< CXXMetaEnum< T > > Get() \
			{ \
				return SP_CAST< CXXMetaEnum< T > >( EnumID< T >::Get() ); \
			} \
		}; \
		template< typename T > struct Class \
		{ \
			static XE::SharedPtr< CXXMetaClass< T > > Get() \
			{ \
				return SP_CAST< CXXMetaClass< T > >( ClassID< T >::Get() ); \
			} \
		}; \
		MetaTypeCollector() \
		{ \
			auto type = std::conditional_t< std::is_enum< ThisType >::value, Enum< ThisType >, Class< ThisType > >::Get();

#define REG_METHOD( NAME, METHOD ) type->Method( NAME, METHOD )
#define REG_PROPERTY( NAME, ... ) type->Property( NAME, __VA_ARGS__ )

#define END_META() \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< ThisType > >::Register(); \
		} \
	}; \
};

#define IMPLEMENT_META(CLASS) BEG_META(CLASS) END_META()


#define OBJECT_2( _CLASS, _SUPER ) \
	template< typename T > friend struct XE::MetaTypeCollector; \
public: \
    typedef _SUPER Super; \
	static XE::MetaClassPtr GetMetaClassStatic() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, ClassID< _SUPER >::Get(), nullptr, GetModule() ); \
		return meta; \
	} \
	virtual XE::MetaClassPtr GetMetaClass() const \
	{ \
		return GetMetaClassStatic(); \
	} \
private:
#define OBJECT_1( _CLASS ) \
	template< typename T > friend struct XE::MetaTypeCollector; \
public: \
	static XE::MetaClassPtr GetMetaClassStatic() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, nullptr, nullptr, GetModule() ); \
		return meta; \
	} \
	virtual XE::MetaClassPtr GetMetaClass() const \
	{ \
		return GetMetaClassStatic(); \
	} \
private:
#define OBJECT(...)            MACRO_EXP_(MACRO_GLUE(OBJECT_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

#endif // __REFLECTION_H__7A09BEF6_4373_410D_94CD_7AF277A37AB3
