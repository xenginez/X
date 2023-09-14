/*!
 * \file   Archive.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __ARCHIVE_H__2AE531B5_C127_4A0B_A947_6B304A884EAA
#define __ARCHIVE_H__2AE531B5_C127_4A0B_A947_6B304A884EAA

#include "MetaEnum.h"
#include "MetaClass.h"
#include "MetaProperty.h"
#include "MetaAttribute.h"
#include "VariantOperation.hpp"

BEG_XE_NAMESPACE

class XE_API ArchiveNameVariant
{
public:
	ArchiveNameVariant( const XE::String & name, const XE::Variant & value )
		: Name( name ), Value( value )
	{
	}

public:
	XE::String Name;
	XE::Variant Value;
};
template< typename T > class ArchiveNameValue
{
public:
	ArchiveNameValue() = default;

	ArchiveNameValue( const XE::String & name, T & value )
		: Name( name ), Value( value )
	{
	}

	ArchiveNameValue( const XE::String & name, const T & value )
		: Name( name ), Value( const_cast<T &>( value ) )
	{
	}

public:
	T & Value;
	XE::String Name;
};

class XE_API OArchive : public XE::NonCopyable
{
protected:
	OArchive() = default;

	virtual ~OArchive() = default;

public:
	template< typename T > OArchive & operator & ( const T & val )
	{
		Serialize( val );

		return *this;
	}

	template< typename T > OArchive & operator & ( XE::ArchiveNameValue< T > val )
	{
		XE::ArchiveNameVariant variant( val.Name, val.Value );

		Serialize( variant );

		return *this;
	}

public:
	template< typename T > OArchive & operator << ( const T & val )
	{
		Serialize( val );

		return *this;
	}

	template< typename T > OArchive & operator << ( XE::ArchiveNameValue< T > val )
	{
		XE::ArchiveNameVariant variant( val.Name, val.Value );

		Serialize( variant );

		return *this;
	}

protected:
	virtual void Serialize( const XE::Variant & val ) = 0;
};

class XE_API IArchive : public XE::NonCopyable
{
protected:
	IArchive() = default;

	virtual ~IArchive() = default;

public:
	template< typename T > IArchive & operator & ( T & val )
	{
		val = Deserialize().Value< T & >();

		return *this;
	}

	template< typename T > IArchive & operator & ( T *& val )
	{
		if( val == nullptr )
		{
			val = Deserialize().Value< T * >();
		}
		else
		{
			*val = Deserialize().Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator & ( XE::SharedPtr< T > & val )
	{
		if( val == nullptr )
		{
			val = Deserialize().Value< XE::SharedPtr< T > >();
		}
		else
		{
			*val = Deserialize().Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator & ( XE::ArchiveNameValue< T > val )
	{
		val.Value = Deserialize( val.Name ).Value< T & >();

		return *this;
	}

	template< typename T > IArchive & operator & ( XE::ArchiveNameValue< T * > val )
	{
		if( val.Value == nullptr )
		{
			val.Value = Deserialize( val.Name ).Value< T * >();
		}
		else
		{
			*val.Value = Deserialize( val.Name ).Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator & ( XE::ArchiveNameValue< XE::SharedPtr< T > > val )
	{
		if( val.Value == nullptr )
		{
			val.Value = Deserialize( val.Name ).Value< XE::SharedPtr< T > >();
		}
		else
		{
			*val.Value = Deserialize( val.Name ).Value< T & >();
		}

		return *this;
	}

public:
	template< typename T > IArchive & operator >> ( T & val )
	{
		val = Deserialize().Value< T & >();

		return *this;
	}

	template< typename T > IArchive & operator >> ( T *& val )
	{
		if( val == nullptr )
		{
			val = Deserialize().Value< T * >();
		}
		else
		{
			*val = Deserialize().Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator >> ( XE::SharedPtr< T > & val )
	{
		if( val == nullptr )
		{
			val = Deserialize().Value< XE::SharedPtr< T > >();
		}
		else
		{
			*val = Deserialize().Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator >> ( XE::ArchiveNameValue< T > val )
	{
		val.Value = Deserialize( val.Name ).Value< T & >();

		return *this;
	}

	template< typename T > IArchive & operator >> ( XE::ArchiveNameValue< T * > val )
	{
		if( val.Value == nullptr )
		{
			val.Value = Deserialize( val.Name ).Value< T * >();
		}
		else
		{
			*val.Value = Deserialize( val.Name ).Value< T & >();
		}

		return *this;
	}

	template< typename T > IArchive & operator >> ( XE::ArchiveNameValue< XE::SharedPtr< T > > val )
	{
		if( val.Value == nullptr )
		{
			val.Value = Deserialize( val.Name ).Value< XE::SharedPtr< T > >();
		}
		else
		{
			*val.Value = Deserialize( val.Name ).Value< T & >();
		}

		return *this;
	}

protected:
	virtual XE::Variant Deserialize( const XE::String & name = "" ) = 0;
};

class XE_API XmlOArchive : public OArchive
{
private:
	struct Private;

public:
	XmlOArchive( std::ostream & stream );

	~XmlOArchive() override;

protected:
	void Serialize( const XE::Variant & val ) override;

private:
	Private * _p;
};

class XE_API XmlIArchive : public IArchive
{
private:
	struct Private;

public:
	XmlIArchive( std::istream & val );

	~XmlIArchive() override;

protected:
	XE::Variant Deserialize( const XE::String & name = "" ) override;

private:
	Private * _p;
};

class XE_API JsonOArchive : public OArchive
{
private:
	struct Private;

public:
	JsonOArchive( std::ostream & stream );

	~JsonOArchive() override;

protected:
	void Serialize( const XE::Variant & val ) override;

private:
	Private * _p;
};

class XE_API JsonIArchive : public IArchive
{
private:
	struct Private;

public:
	JsonIArchive( std::istream & val );

	~JsonIArchive() override;

protected:
	XE::Variant Deserialize( const XE::String & name = "" ) override;

private:
	Private * _p;
};

class XE_API BinaryOArchive : public OArchive
{
private:
	struct Private;

public:
	BinaryOArchive( std::ostream & stream );

	~BinaryOArchive() override;

protected:
	void Serialize( const XE::Variant & val ) override;

private:
	Private * _p;
};

class XE_API BinaryIArchive : public IArchive
{
private:
	struct Private;

public:
	BinaryIArchive( std::istream & val );

	~BinaryIArchive() override;

protected:
	XE::Variant Deserialize( const XE::String & name = "" ) override;

private:
	Private * _p;
};

#define ARCHIVE_NVP_1( VALUE ) XE::ArchiveNameValue< typename XE::TypeTraits< decltype( VALUE ) >::remove_const_volatile_reference_t >( #VALUE, VALUE )
#define ARCHIVE_NVP_2( NAME, VALUE ) XE::ArchiveNameValue< typename XE::TypeTraits< decltype( VALUE ) >::remove_const_volatile_reference_t >( NAME, VALUE )
#define ARCHIVE_NVP( ... ) MACRO_EXP_(MACRO_GLUE(ARCHIVE_NVP_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

template< typename T > struct Cloneable
{
public:
	template< typename U > struct HasMemberClone
	{
	private:
		template < typename Ty, void( Ty:: * )( Ty * ) const = &Ty::Clone >
		static constexpr auto check( Ty * )
		{
			return true;
		};

		static constexpr bool check( ... )
		{
			return false;
		};

	public:
		static constexpr bool value = check( static_cast<U *>( nullptr ) );
	};

public:
	static void Clone( const T * from, T * to )
	{
		if constexpr( HasMemberClone< T >::value )
		{
			from->Clone( to );
		}
		else
		{
			::ClassID< T >::Get( from )->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
			{
				if( prop->FindAttributeT< XE::NonCloneAttribute >() == nullptr )
				{
					prop->Set( to, prop->Get( from ) );
				}
			} );
		}
	}
};

template< typename T > struct Serializable
{
public:
	template< typename U > struct HasMemberSerialize
	{
	private:
		template < typename Ty, void( Ty:: * )( XE::OArchive & ) const = &Ty::Serialize >
		static constexpr auto check( Ty * )
		{
			return true;
		};

		static constexpr bool check( ... )
		{
			return false;
		};

	public:
		static constexpr bool value = check( static_cast<U *>( nullptr ) );
	};

	template< typename U > struct HasMemberDeserialize
	{
	private:
		template < typename Ty, void( Ty:: * )( XE::IArchive & ) = &Ty::Deserialize >
		static constexpr auto check( Ty * )
		{
			return true;
		};

		static constexpr bool check( ... )
		{
			return false;
		};

	public:
		static constexpr bool value = check( static_cast<U *>( nullptr ) );
	};

public:
	static void Serialize( XE::OArchive & arc, const T & val )
	{
		if constexpr( HasMemberSerialize< T >::value )
		{
			val.Serialize( arc );
		}
		else if constexpr( std::is_enum_v< T > )
		{
			arc << val;
		}
		else
		{
			if( auto cls = ::ClassID< T >::Get( &val ) )
			{
				cls->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
				{
					if( !prop->IsStatic() && prop->FindAttributeT< XE::NonSerializeAttribute >() == nullptr )
					{
						arc << ARCHIVE_NVP( prop->GetName(), prop->Get( &val ) );
					}
				} );
			}
		}
	}

	static void Deserialize( XE::IArchive & arc, T & val )
	{
		if constexpr( HasMemberDeserialize< T >::value )
		{
			val.Deserialize( arc );
		}
		else if constexpr( std::is_enum_v< T > )
		{
			arc >> val;
		}
		else
		{
			if( auto cls = ::ClassID< T >::Get( &val ) )
			{
				cls->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
				{
					if( !prop->IsStatic() && prop->FindAttributeT< XE::NonSerializeAttribute >() == nullptr )
					{
						XE::Variant var = prop->Get( val );

						auto nvp = ARCHIVE_NVP( prop->GetName(), var );

						arc >> nvp;

						prop->Set( &val, var );
					}
				} );
			}
		}
	}
};


template< typename ... Types > struct Serializable< std::list< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::list< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::list< Types... > & val )
	{
		typename std::list< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::list< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.emplace_back( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::deque< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::deque< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::deque< Types... > & val )
	{
		typename std::deque< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::deque< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.emplace_back( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::stack< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::stack< Types... > & val )
	{
		std::stack< Types... > temp = val;

		arc << ARCHIVE_NVP( "size", temp.size() );

		XE::uint64 i = 0;
		while( !temp.empty() )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), temp.top() );
			temp.pop();
		}
	}

	static void Deserialize( XE::IArchive & arc, std::stack< Types... > & val )
	{
		std::vector< typename std::stack< Types... >::value_type > temp;

		typename std::stack< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::vector< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			temp.emplace_back( std::move( value ) );
		}

		for( auto it = temp.rbegin(); it != temp.rend(); ++it )
		{
			val.push( *it );
		}
	}
};
template< typename ... Types > struct Serializable< std::queue< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::queue< Types... > & val )
	{
		std::queue< Types... > temp = val;

		arc << ARCHIVE_NVP( "size", temp.size() );

		XE::uint64 i = 0;
		while( !temp.empty() )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), temp.front() );
			temp.pop();
		}
	}

	static void Deserialize( XE::IArchive & arc, std::queue< Types... > & val )
	{
		typename std::queue< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::queue< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.push( value );
		}
	}
};
template< typename ... Types > struct Serializable< std::vector< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::vector< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::vector< Types... > & val )
	{
		typename std::vector< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::vector< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.emplace_back( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::pair< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::pair< Types... > & val )
	{
		auto key = ARCHIVE_NVP( "first", val.first );
		arc << key;

		auto value = ARCHIVE_NVP( "second", val.second );
		arc << value;
	}

	static void Deserialize( XE::IArchive & arc, std::pair< Types... > & val )
	{
		arc >> ARCHIVE_NVP( "first", val.first );

		arc >> ARCHIVE_NVP( "second", val.second );
	}
};
template< typename ... Types > struct Serializable< std::set< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::set< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::set< Types... > & val )
	{
		typename std::set< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::set< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::map< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::map< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			std::pair< std::map< Types... >::key_type, std::map< Types... >::mapped_type > value( it );
			arc << ARCHIVE_NVP( XE::ToString( i++ ), value );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::map< Types... > & val )
	{
		typename std::map< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			std::pair< std::map< Types... >::key_type, std::map< Types... >::mapped_type > value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::multiset< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::multiset< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::multiset< Types... > & val )
	{
		typename std::multiset< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::multiset< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::multimap< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::multimap< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			std::pair< std::multimap< Types... >::key_type, std::multimap< Types... >::mapped_type > value( it );
			arc << ARCHIVE_NVP( XE::ToString( i++ ), value );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::multimap< Types... > & val )
	{
		typename std::multimap< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			std::pair< std::multimap< Types... >::key_type, std::multimap< Types... >::mapped_type > value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::unordered_set< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::unordered_set< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::unordered_set< Types... > & val )
	{
		typename std::unordered_set< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::unordered_set< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::unordered_map< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::unordered_map< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			std::pair< std::unordered_map< Types... >::key_type, std::unordered_map< Types... >::mapped_type > value( it );
			arc << ARCHIVE_NVP( XE::ToString( i++ ), value );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::unordered_map< Types... > & val )
	{
		typename std::unordered_map< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			std::pair< std::unordered_map< Types... >::key_type, std::unordered_map< Types... >::mapped_type > value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::unordered_multiset< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::unordered_multiset< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			arc << ARCHIVE_NVP( XE::ToString( i++ ), it );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::unordered_multiset< Types... > & val )
	{
		typename std::unordered_multiset< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			typename std::unordered_multiset< Types... >::value_type value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};
template< typename ... Types > struct Serializable< std::unordered_multimap< Types... > >
{
	static void Serialize( XE::OArchive & arc, const std::unordered_multimap< Types... > & val )
	{
		arc << ARCHIVE_NVP( "size", val.size() );

		XE::uint64 i = 0;
		for( const auto & it : val )
		{
			std::pair< std::unordered_multimap< Types... >::key_type, std::unordered_multimap< Types... >::mapped_type > value( it );
			arc << ARCHIVE_NVP( XE::ToString( i++ ), value );
		}
	}

	static void Deserialize( XE::IArchive & arc, std::unordered_multimap< Types... > & val )
	{
		typename std::unordered_multimap< Types... >::size_type size = 0;
		arc >> ARCHIVE_NVP( "size", size );

		for( XE::uint64 i = 0; i < size; ++i )
		{
			std::pair< std::unordered_multimap< Types... >::key_type, std::unordered_multimap< Types... >::mapped_type > value;
			arc >> ARCHIVE_NVP( XE::ToString( i ), value );
			val.insert( std::move( value ) );
		}
	}
};

END_XE_NAMESPACE

#endif // __ARCHIVE_H__2AE531B5_C127_4A0B_A947_6B304A884EAA
