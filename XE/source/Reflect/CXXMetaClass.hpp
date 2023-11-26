/*!
 * \file   CXXMetaClass.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CXXMETACLASS_HPP__B0EBC0E5_97E9_4A1F_B8EE_EF87D334D01A
#define __CXXMETACLASS_HPP__B0EBC0E5_97E9_4A1F_B8EE_EF87D334D01A

#include "Memory/MStream.hpp"

#include "MetaClass.h"

#include "Archive.h"
#include "MetaModule.h"
#include "MetaAttribute.h"
#include "CXXMetaMethod.hpp"
#include "CXXMetaProperty.hpp"
#include "VariantOperation.hpp"
#include "CXXMetaEnumerator.hpp"

BEG_XE_NAMESPACE

template< typename ClassType > class CXXMetaClass : public XE::MetaClass
{
public:
	CXXMetaClass( const XE::String & Name, XE::MetaClassCPtr Super, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module, const XE::TemplateType temps = {}, const XE::MetaTypeCPtr & element = nullptr )
		: XE::MetaClass( Name, sizeof( ClassType ), std::is_abstract_v< ClassType >, std::is_container_v< ClassType >, Super, Owner, Module, temps, element )
	{
	}

public:
	void Destruct( void * ptr ) const override
	{
		if constexpr( !std::is_abstract_v< ClassType > )
		{
			if constexpr( std::is_constructible_v< ClassType > )
			{
				( (ClassType *) ( ptr ) )->~ClassType();
			}
		}
	}

	XE::Variant Construct() const override
	{
		if constexpr ( !std::is_abstract_v< ClassType > && std::is_constructible_v< ClassType > )
		{
			return XE::MakeShared< ClassType >();
		}

		throw XE::MetaException( shared_from_this(), "is abstract type!" );
	}

	XE::Variant Construct( void * ptr ) const override
	{
		if constexpr( !std::is_abstract_v< ClassType > && std::is_constructible_v< ClassType > )
		{
			if( ptr == nullptr )
			{
				ptr = XE::MemoryResource::Alloc( GetSize() );
			}

			return new ( ptr ) ClassType();
		}

		throw XE::MetaException( shared_from_this(), "is abstract type!" );
	}

	void Clone( const XE::Variant & from, XE::Variant & to ) const override
	{
		XE::Cloneable< ClassType >::Clone( from.Value< const ClassType * >(), to.Value< ClassType * >() );
	}

	XE::MetaEnumeratorPtr GetEnumerator( const XE::Variant & container ) const override
	{
		return nullptr;
	}

	void Serialize( XE::OArchive & arc, const XE::Variant & val ) const override
	{
		if constexpr( !std::is_abstract_v< ClassType > && std::is_constructible_v< ClassType > && std::is_copy_constructible_v< ClassType > )
		{
			XE::Serializable< ClassType >::Serialize( arc, val.Value< const ClassType & >() );
		}
	}

	void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const override
	{
		if constexpr( !std::is_abstract_v< ClassType > && std::is_constructible_v< ClassType > && std::is_copy_constructible_v< ClassType > )
		{
			XE::Serializable< ClassType >::Deserialize( arc, obj.Value< ClassType & >() );
		}
	}

public:
	template< typename Result, typename ... T > auto Method( const XE::String & Name, Result( *Callback )( T... ) )
	{
		auto method = XE::MetaInfo::NewMetaInfo< XE::CXXMetaMethod< Result( * )( T... ) > >( 0, Name, Callback, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterMethod( method );
		return method;
	}

	template< typename Result, typename ... T > auto Method( const XE::String & Name, Result( ClassType:: * Callback )( T... ) )
	{
		auto method = XE::MetaInfo::NewMetaInfo< CXXMetaMethod< Result( ClassType:: * )( T... ) > >( 0, Name, Callback, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterMethod( method );
		return method;
	}

	template< typename Result, typename ... T > auto Method( const XE::String & Name, Result( ClassType:: * Callback )( T... ) const )
	{
		auto method = XE::MetaInfo::NewMetaInfo< XE::CXXMetaMethod< Result( ClassType:: * )( T... ) const > >( 0, Name, Callback, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterMethod( method );
		return method;
	}

	template< typename Value > auto Property( const XE::String & Name, Value * Prop )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< Value > >( 0, Name, Prop, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename Value > auto Property( const XE::String & Name, Value( ClassType:: * Prop ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< Value( ClassType:: * ) > >( 0, Name, Prop, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType, typename SetType > auto Property( const XE::String & Name, GetType( *Get )( ), void( *Set )( SetType ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< GetType( * )( ), void( * )( SetType ) > >( 0, Name, Get, Set, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType, typename SetType > auto Property( const XE::String & Name, GetType( ClassType:: * Get )( ), void( ClassType:: * Set )( SetType ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< GetType( ClassType:: * )( ), void( ClassType:: * )( SetType ) > >( 0, Name, Get, Set, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType, typename SetType > auto Property( const XE::String & Name, GetType( ClassType:: * Get )( ) const, void( ClassType:: * Set )( SetType ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< CXXMetaProperty< GetType( ClassType:: * )( ) const, void( ClassType:: * )( SetType ) > >( 0, Name, Get, Set, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType > auto Property( const XE::String & Name, GetType( *Get )( ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< GetType( * )( ) > >( 0, Name, Get, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType > auto Property( const XE::String & Name, GetType( ClassType:: * Get )( ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< CXXMetaProperty< GetType( ClassType:: * )( ) > >( 0, Name, Get, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename GetType > auto Property( const XE::String & Name, GetType( ClassType:: * Get )( ) const )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< GetType( ClassType:: * )( ) const > >( 0, Name, Get, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename SetType > auto Property( const XE::String & Name, void( *Set )( SetType ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< void( * )( SetType ) > >( 0, Name, Set, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

	template< typename SetType > auto Property( const XE::String & Name, void( ClassType:: * Set )( SetType ) )
	{
		auto prop = XE::MetaInfo::NewMetaInfo< XE::CXXMetaProperty< void( ClassType:: * )( SetType ) > >( 0, Name, Set, SP_CAST< XE::MetaClass >( shared_from_this() ), GetModule() );
		_RegisterProperty( prop );
		return prop;
	}

};

template< typename ClassType > class CXXMetaFundamental : public XE::MetaClass
{
public:
	CXXMetaFundamental( const XE::String & Name )
		: XE::MetaClass( Name, sizeof( ClassType ), false, false, nullptr, nullptr, nullptr, {}, nullptr )
	{
	}

public:
	void Destruct( void * ptr ) const override
	{

	}

	XE::Variant Construct() const override
	{
		if constexpr ( !std::is_abstract_v< ClassType > && std::is_constructible_v< ClassType > )
		{
			return ClassType();
		}

		throw XE::MetaException( shared_from_this(), "is abstract type!" );
	}

	XE::Variant Construct( void * ptr ) const override
	{
		if( ptr == nullptr )
		{
			ptr = XE::MemoryResource::Alloc( GetSize() );
		}

		return new ( ptr ) ClassType();
	}

	void Clone( const XE::Variant & from, XE::Variant & to ) const override
	{
		to = from.Value< ClassType >();
	}

	XE::MetaEnumeratorPtr GetEnumerator( const XE::Variant & container ) const override
	{
		return nullptr;
	}

	void Serialize( XE::OArchive & arc, const XE::Variant & val ) const override
	{
		XE::Serializable< ClassType >::Serialize( arc, val.Value< const ClassType & >() );
	}

	void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const override
	{
		XE::Serializable< ClassType >::Deserialize( arc, obj.Value< ClassType & >() );
	}
};

template< typename ClassType, typename ... Types > class CXXTplMetaClass : public XE::CXXMetaClass< ClassType >
{
public:
	CXXTplMetaClass( const XE::String & Name, XE::MetaClassCPtr Super, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::CXXMetaClass< ClassType >( Name + XE::ToString( XE::MakeTemplateType< Types... >() ), Super, Owner, Module, XE::MakeTemplateType< Types... >(), ::TypeID< std::is_container< ClassType >::value_type >::Get() )
	{
	}

public:
	XE::MetaEnumeratorPtr GetEnumerator( const XE::Variant & container ) const override
	{
		if constexpr( std::is_container_v< ClassType > )
		{
			return XE::MakeShared< XE::CXXMetaEnumerator< ClassType > >( container.Value< ClassType *>() );
		}
		else
		{
			return nullptr;
		}
	}
};



template<> struct ::ClassID< void >
{
	static XE::MetaClassCPtr Get( const void * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< std::nullptr_t > >( XE::Hash( __xe__sig__ ), "void" );
		return meta;
	}
};

template<> struct ::ClassID< std::nullptr_t >
{
	static XE::MetaClassCPtr Get( const std::nullptr_t * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< std::nullptr_t > >( XE::Hash( __xe__sig__ ), "null" );
		return meta;
	}
};

template<> struct ::ClassID< bool >
{
	static XE::MetaClassCPtr Get( const bool * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< bool > >( XE::Hash( __xe__sig__ ), "bool" );
		return meta;
	}
};

template<> struct ::ClassID< XE::int8 >
{
	static XE::MetaClassCPtr Get( const XE::int8 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::int8 > >( XE::Hash( __xe__sig__ ), "int8" );
		return meta;
	}
};

template<> struct ::ClassID< XE::int16 >
{
	static XE::MetaClassCPtr Get( const XE::int16 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::int16 > >( XE::Hash( __xe__sig__ ), "int16" );
		return meta;
	}
};

template<> struct ::ClassID< XE::int32 >
{
	static XE::MetaClassCPtr Get( const XE::int32 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::int32 > >( XE::Hash( __xe__sig__ ), "int32" );
		return meta;
	}
};

template<> struct ::ClassID< long >
{
	static XE::MetaClassCPtr Get( const long * val = nullptr )
	{
		return ::ClassID< XE::int32 >::Get();
	}
};

template<> struct ::ClassID< XE::int64 >
{
	static XE::MetaClassCPtr Get( const XE::int64 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::int64 > >( XE::Hash( __xe__sig__ ), "int64" );
		return meta;
	}
};

template<> struct ::ClassID< XE::uint8 >
{
	static XE::MetaClassCPtr Get( const XE::uint8 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::uint8 > >( XE::Hash( __xe__sig__ ), "uint8" );
		return meta;
	}
};

template<> struct ::ClassID< XE::uint16 >
{
	static XE::MetaClassCPtr Get( const XE::uint16 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::uint16 > >( XE::Hash( __xe__sig__ ), "uint16" );
		return meta;
	}
};

template<> struct ::ClassID< XE::uint32 >
{
	static XE::MetaClassCPtr Get( const XE::uint32 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::uint32 > >( XE::Hash( __xe__sig__ ), "uint32" );
		return meta;
	}
};

template<> struct ::ClassID< unsigned long >
{
	static XE::MetaClassCPtr Get( const unsigned long * val = nullptr )
	{
		return ::ClassID< XE::uint32 >::Get();
	}
};

template<> struct ::ClassID< XE::uint64 >
{
	static XE::MetaClassCPtr Get( const XE::uint64 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::uint64 > >( XE::Hash( __xe__sig__ ), "uint64" );
		return meta;
	}
};

template<> struct ::ClassID< XE::float32 >
{
	static XE::MetaClassCPtr Get( const XE::float32 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::float32 > >( XE::Hash( __xe__sig__ ), "float32" );
		return meta;
	}
};

template<> struct ::ClassID< XE::float64 >
{
	static XE::MetaClassCPtr Get( const XE::float64 * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFundamental< XE::float64 > >( XE::Hash( __xe__sig__ ), "float64" );
		return meta;
	}
};

template<> struct ::ClassID< XE::String >
{
	static XE::MetaClassCPtr Get( const XE::String * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::String > >( XE::Hash( __xe__sig__ ), "String", nullptr, nullptr, XE::GetModule() );

		return meta;
	}
};

template<> struct ::ClassID< XE::AnsiString >
{
	static XE::MetaClassCPtr Get( const XE::String * val = nullptr )
	{
		return ::ClassID< XE::String >::Get();
	}
};

template<> struct ::ClassID< XE::WideString >
{
	static XE::MetaClassCPtr Get( const XE::WideString * val = nullptr )
	{
		return ::ClassID< XE::String >::Get();
	}
};

template<> struct ::ClassID< XE::Variant >
{
	static XE::MetaClassCPtr Get( const XE::Variant * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::Variant > >( XE::Hash( __xe__sig__ ), "Variant", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::MetaID >
{
	static XE::MetaClassCPtr Get( const XE::MetaID * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::MetaID > >( XE::Hash( __xe__sig__ ), "MetaID", nullptr, nullptr, XE::GetModule() );

		return meta;
	}
};

template< typename T > struct ::ClassID< XE::Hasher< T > >
{
	static XE::MetaClassCPtr Get( const XE::Hasher< T > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< std::filesystem::path > >( XE::Hash( __xe__sig__ ), "Hasher", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< XE::BasicMemoryView< Types... > >
{
	static XE::MetaClassCPtr Get( const XE::BasicMemoryView< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< XE::BasicMemoryView< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "MemoryView", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};



template<> struct ::ClassID< XE::VariantList >
{
	static XE::MetaClassCPtr Get( const XE::VariantList * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantList > >( XE::Hash( __xe__sig__ ), "List", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantDeque >
{
	static XE::MetaClassCPtr Get( const XE::VariantDeque * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantDeque > >( XE::Hash( __xe__sig__ ), "Deque", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantStack >
{
	static XE::MetaClassCPtr Get( const XE::VariantStack * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantStack > >( XE::Hash( __xe__sig__ ), "Stack", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantQueue >
{
	static XE::MetaClassCPtr Get( const XE::VariantQueue * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantQueue > >( XE::Hash( __xe__sig__ ), "Queue", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantArray >
{
	static XE::MetaClassCPtr Get( const XE::VariantArray * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantArray > >( XE::Hash( __xe__sig__ ), "Array", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantPair >
{
	static XE::MetaClassCPtr Get( const XE::VariantPair * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantPair > >( XE::Hash( __xe__sig__ ), "Pair", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantSet >
{
	static XE::MetaClassCPtr Get( const XE::VariantSet * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantSet > >( XE::Hash( __xe__sig__ ), "Set", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantMap >
{
	static XE::MetaClassCPtr Get( const XE::VariantMap * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantMap > >( XE::Hash( __xe__sig__ ), "Map", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantMultiSet >
{
	static XE::MetaClassCPtr Get( const XE::VariantMultiSet * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantMultiSet > >( XE::Hash( __xe__sig__ ), "MultiSet", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantMultiMap >
{
	static XE::MetaClassCPtr Get( const XE::VariantMultiMap * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantMultiMap > >( XE::Hash( __xe__sig__ ), "MultiMap", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantUnorderedSet >
{
	static XE::MetaClassCPtr Get( const XE::VariantUnorderedSet * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantUnorderedSet > >( XE::Hash( __xe__sig__ ), "UnorderedSet", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantUnorderedMap >
{
	static XE::MetaClassCPtr Get( const XE::VariantUnorderedMap * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantUnorderedMap > >( XE::Hash( __xe__sig__ ), "UnorderedMap", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantUnorderedMultiSet >
{
	static XE::MetaClassCPtr Get( const XE::VariantUnorderedMultiSet * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantUnorderedMultiSet > >( XE::Hash( __xe__sig__ ), "UnorderedMultiSet", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::VariantUnorderedMultiMap >
{
	static XE::MetaClassCPtr Get( const XE::VariantUnorderedMultiMap * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::VariantUnorderedMultiMap > >( XE::Hash( __xe__sig__ ), "UnorderedMultiMap", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template<> struct ::ClassID< XE::ArchiveNameVariant >
{
	static XE::MetaClassCPtr Get( const XE::ArchiveNameVariant * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< XE::ArchiveNameVariant > >( XE::Hash( __xe__sig__ ), "ArchiveNameVariant", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};



template<> struct ::ClassID< std::filesystem::path >
{
	static XE::MetaClassCPtr Get( const std::filesystem::path * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< std::filesystem::path > >( XE::Hash( __xe__sig__ ), "Path", nullptr, nullptr, XE::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::less< Types... > >
{
	static XE::MetaClassCPtr Get( const std::less< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::less< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "less", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::hash< Types... > >
{
	static XE::MetaClassCPtr Get( const std::hash< Types... > * val = nullptr )
	{
		return ::ClassID< XE::Hasher< Types... > >::Get( val );
	}
};

template< typename ... Types > struct ::ClassID< std::equal_to< Types... > >
{
	static XE::MetaClassCPtr Get( const std::equal_to< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::equal_to< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "equal_to", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::allocator< Types... > >
{
	static XE::MetaClassCPtr Get( const std::allocator< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::allocator< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "allocator", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::pmr::polymorphic_allocator< Types... > >
{
	static XE::MetaClassCPtr Get( const std::pmr::polymorphic_allocator< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::pmr::polymorphic_allocator< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "polymorphic_allocator", nullptr, nullptr, std::pmr::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::list< Types... > >
{
	static XE::MetaClassCPtr Get( const std::list< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::list< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "list", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::deque< Types... > >
{
	static XE::MetaClassCPtr Get( const std::deque< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::deque< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "deque", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::stack< Types... > >
{
	static XE::MetaClassCPtr Get( const std::stack< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::stack< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "stack", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::queue< Types... > >
{
	static XE::MetaClassCPtr Get( const std::queue< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::queue< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "queue", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::vector< Types... > >
{
	static XE::MetaClassCPtr Get( const std::vector< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::vector< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "vector", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename K, typename V > struct ::ClassID< std::pair< K, V > >
{
	static XE::MetaClassCPtr Get( const std::pair< K, V > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::pair< K, V >, K, V > >( XE::Hash( __xe__sig__ ), "pair", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename K, typename V > struct ::ClassID< std::pair< const K, V > >
{
	static XE::MetaClassCPtr Get( const std::pair< const K, V > * val = nullptr )
	{
		return ClassID< std::pair< K, V > >::Get();
	}
};

template< typename ... Types > struct ::ClassID< std::set< Types... > >
{
	static XE::MetaClassCPtr Get( const std::set< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::set< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "set", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::map< Types... > >
{
	static XE::MetaClassCPtr Get( const std::map< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::map< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "map", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::multiset< Types... > >
{
	static XE::MetaClassCPtr Get( const std::multiset< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::multiset< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "multiset", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::multimap< Types... > >
{
	static XE::MetaClassCPtr Get( const std::multimap< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::multimap< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "multimap", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::unordered_set< Types... > >
{
	static XE::MetaClassCPtr Get( const std::unordered_set< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::unordered_set< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "unordered_set", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::unordered_map< Types... > >
{
	static XE::MetaClassCPtr Get( const std::unordered_map< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::unordered_map< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "unordered_map", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::unordered_multiset< Types... > >
{
	static XE::MetaClassCPtr Get( const std::unordered_multiset< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::unordered_multiset< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "unordered_multiset", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};

template< typename ... Types > struct ::ClassID< std::unordered_multimap< Types... > >
{
	static XE::MetaClassCPtr Get( const std::unordered_multimap< Types... > * val = nullptr )
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< std::unordered_multimap< Types... >, Types... > >( XE::Hash( __xe__sig__ ), "unordered_multimap", nullptr, nullptr, std::GetModule() );
		return meta;
	}
};



template<> struct ::TypeID< XE::Variant >
{
	static XE::MetaTypeCPtr Get( const XE::Variant * val = nullptr )
	{
		if( val != nullptr && val->IsInvalid() == false )
		{
			return val->GetType();
		}

		return ::ClassID< XE::Variant >::Get();
	}
};

END_XE_NAMESPACE

#define DECL_META_CLASS_3( _MODULE, _CLASS, _SUPPER ) \
template<> struct ::ClassID< _MODULE::_CLASS > \
{ \
	static XE::MetaClassCPtr Get( const _MODULE::_CLASS * val = nullptr ) \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _MODULE::_CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, ::ClassID< _SUPPER >::Get(), nullptr, _MODULE##::GetModule() ); \
		return meta; \
	} \
}
#define DECL_META_CLASS_2( _MODULE, _CLASS ) \
template<> struct ::ClassID< _MODULE::_CLASS > \
{ \
	static XE::MetaClassCPtr Get( const _MODULE::_CLASS * val = nullptr ) \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _MODULE::_CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, nullptr, nullptr, _MODULE##::GetModule() ); \
		return meta; \
	} \
}
#define DECL_META_CLASS_1( _CLASS ) \
template<> struct ::ClassID< _CLASS > \
{ \
	static XE::MetaClassCPtr Get( const _CLASS * val = nullptr ) \
	{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaClass< _CLASS > >( XE::Hash( __xe__sig__ ), #_CLASS, nullptr, nullptr, ::GetModule() ); \
		return meta; \
	} \
}
#define DECL_META_CLASS( ... ) MACRO_EXP_(MACRO_GLUE(DECL_META_CLASS_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))




#define BEG_META_TEMPLATE_CLASS_3( _MODULE, _CLASS, _SUPPER ) \
template< typename ... Types > struct ::ClassID< _MODULE::_CLASS< Types... > > \
{ \
static XE::MetaClassCPtr Get( const _MODULE::_CLASS< Types... > * val = nullptr ) \
{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = [](){ \
									using ThisType = _MODULE::_CLASS< Types... >; \
									auto type = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< _MODULE::_CLASS< Types... >, Types... > >( XE::Hash( __xe__sig__ ), #_CLASS, ::ClassID< _SUPPER >::Get(), nullptr, _MODULE##::GetModule() );

#define BEG_META_TEMPLATE_CLASS_2( _MODULE, _CLASS ) \
template< typename ... Types > struct ::ClassID< _MODULE::_CLASS< Types... > > \
{ \
static XE::MetaClassCPtr Get( const _MODULE::_CLASS< Types... > * val = nullptr ) \
{ \
		static constexpr char __xe__sig__[] = __FUNCTION__; \
		static auto meta = [](){ \
									using ThisType = _MODULE::_CLASS< Types... >; \
									auto type = XE::MetaInfo::NewMetaInfo< XE::CXXTplMetaClass< _MODULE::_CLASS< Types... >, Types... > >( XE::Hash( __xe__sig__ ), #_CLASS, nullptr, nullptr, _MODULE##::GetModule() );

#define END_META_TEMPLATE_CLASS() \
									return type; \
								}(); \
		return meta; \
	} \
}

#define BEG_META_TEMPLATE_CLASS( ... ) MACRO_EXP_(MACRO_GLUE(BEG_META_TEMPLATE_CLASS_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

#define IMPLEMENT_META_TEMPLATE_CLASS( ... ) BEG_META_TEMPLATE_CLASS( __VA_ARGS__ ) END_META_TEMPLATE_CLASS()


#define DECL_XE_CLASS( ... ) DECL_META_CLASS( XE, __VA_ARGS__ )

#define BEG_XE_TEMPLATE_CLASS( ... ) BEG_META_TEMPLATE_CLASS( XE, __VA_ARGS__ )
#define END_XE_TEMPLATE_CLASS() END_META_TEMPLATE_CLASS()
#define IMPLEMENT_XE_TEMPLATE_CLASS( ... ) BEG_XE_TEMPLATE_CLASS( __VA_ARGS__ ) END_XE_TEMPLATE_CLASS()


#define DECL_XE_CLASS( ... ) DECL_META_CLASS( XE, __VA_ARGS__ )

#endif // __CXXMETACLASS_HPP__B0EBC0E5_97E9_4A1F_B8EE_EF87D334D01A
