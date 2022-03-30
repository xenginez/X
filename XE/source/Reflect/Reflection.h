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
	static void VisitMeta( const XE::Delegate< void( const XE::MetaInfoCPtr & ) > & val );

	static void VisitType( const XE::Delegate< void( const XE::MetaTypeCPtr & ) > & val );

	static void VisitEnum( const XE::Delegate< void( const XE::MetaEnumCPtr & ) > & val );

	static void VisitClass( const XE::Delegate< void( const XE::MetaClassCPtr & ) > & val );

	static void VisitModule( const XE::Delegate< void( const XE::MetaModuleCPtr & ) > & val );

	static void VisitMethod( const  XE::Delegate< void( const XE::MetaMethodCPtr & ) > & val );

	static void VisitProperty( const XE::Delegate< void( const  XE::MetaPropertyCPtr & ) > & val );

	static void VisitFunction( const XE::Delegate< void( const  XE::MetaFunctionCPtr & ) > & val );

	static void VisitVariable( const XE::Delegate< void( const  XE::MetaVariableCPtr & ) > & val );

public:
	static XE::MetaInfoCPtr FindMeta( XE::uint64 HashCode );

	static XE::MetaTypeCPtr FindType( XE::uint64 HashCode );

	static XE::MetaEnumCPtr FindEnum( XE::uint64 HashCode );

	static XE::MetaClassCPtr FindClass( XE::uint64 HashCode );

	static XE::MetaModuleCPtr FindModule( XE::uint64 HashCode );

	static XE::MetaMethodCPtr FindMethod( XE::uint64 HashCode );

	static XE::MetaPropertyCPtr FindProperty( XE::uint64 HashCode );

	static XE::MetaFunctionCPtr FindFunction( XE::uint64 HashCode );

	static XE::MetaVariableCPtr FindVariable( XE::uint64 HashCode );

public:
	static XE::MetaInfoCPtr FindMeta( const XE::String & FullName );

	static XE::MetaTypeCPtr FindType( const XE::String & FullName );

	static XE::MetaEnumCPtr FindEnum( const XE::String & FullName );

	static XE::MetaClassCPtr FindClass( const XE::String & FullName );

	static XE::MetaModuleCPtr FindModule( const XE::String & FullName );

	static XE::MetaMethodCPtr FindMethod( const XE::String & FullName );

	static XE::MetaPropertyCPtr FindProperty( const XE::String & FullName );

	static XE::MetaFunctionCPtr FindFunction( const XE::String & FullName );

	static XE::MetaVariableCPtr FindVariable( const XE::String & FullName );

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
				return SP_CAST< CXXMetaEnum< T > >( CP_CAST< XE::MetaEnum >( EnumID< T >::Get() ) ); \
			} \
		}; \
		template< typename T > struct Class \
		{ \
			static XE::SharedPtr< CXXMetaClass< T > > Get() \
			{ \
				return SP_CAST< CXXMetaClass< T > >( CP_CAST< XE::MetaClass >( ClassID< T >::Get() ) ); \
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
	static XE::MetaClassCPtr GetMetaClassStatic() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, ClassID< _SUPER >::Get(), nullptr, GetModule() ); \
		return meta; \
	} \
	virtual XE::MetaClassCPtr GetMetaClass() const \
	{ \
		return GetMetaClassStatic(); \
	} \
private:
#define OBJECT_1( _CLASS ) \
	template< typename T > friend struct XE::MetaTypeCollector; \
public: \
	static XE::MetaClassCPtr GetMetaClassStatic() \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, nullptr, nullptr, GetModule() ); \
		return meta; \
	} \
	virtual XE::MetaClassCPtr GetMetaClass() const \
	{ \
		return GetMetaClassStatic(); \
	} \
private:
#define OBJECT(...)            MACRO_EXP_(MACRO_GLUE(OBJECT_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

#endif // __REFLECTION_H__7A09BEF6_4373_410D_94CD_7AF277A37AB3
