#include "Archive.h"

#include "Reflection.h"

#include <pugixml/pugixml.hpp>
#include <pugixml/pugixml.cpp>

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>

namespace
{
	static constexpr XE::uint8 is_null = 0;
	static constexpr XE::uint8 is_pointer = 1;
	static constexpr XE::uint8 is_shared_ptr = 2;

	const char * flag_str( XE::uint8 flag )
	{
		switch( flag )
		{
		case is_null:
			return "null";
		case is_pointer:
			return "pointer";
		case is_shared_ptr:
			return "shared_ptr";
		default:
			return "unknown";
		}
	}

	XE::uint8 str_flag( const char * str )
	{
		if( std::strcmp( str, "null" ) == 0 )
		{
			return is_null;
		}
		else if( std::strcmp( str, "pointer" ) == 0 )
		{
			return is_pointer;
		}
		else if( std::strcmp( str, "shared_ptr" ) == 0 )
		{
			return is_shared_ptr;
		}

		return std::numeric_limits<XE::uint8>::max();
	}

	void read( std::istream & stream, XE::int8 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::int16 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::int32 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::int64 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::uint8 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::uint16 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::uint32 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::uint64 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::float32 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::float64 & val )
	{
		char buf[sizeof( val )];
		stream.read( buf, sizeof( val ) );
		XE::ReadLittleEndian( buf, val );
	}
	void read( std::istream & stream, XE::String & val )
	{
		XE::String::size_type size = 0;
		read( stream, size );

		val.resize( size );

		stream.read( val.data(), size );
	}
	template< typename T > void read( std::istream & stream, T & val )
	{
		stream.read( reinterpret_cast< char * >( &val ), sizeof( T ) );
	}

	void write( std::ostream & stream, XE::int8 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::int16 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::int32 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::int64 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::uint8 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::uint16 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::uint32 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::uint64 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::float32 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, XE::float64 val )
	{
		char buf[sizeof( val )];
		XE::WriteLittleEndian( buf, val );
		stream.write( buf, sizeof( val ) );
	}
	void write( std::ostream & stream, const XE::String & val )
	{
		write( stream, val.size() );

		stream.write( val.c_str(), val.size() );
	}
	template< typename T > void write( std::ostream & stream, const T & val )
	{
		stream.write( reinterpret_cast< const char * >( &val ), sizeof( T ) );
	}
}

struct XE::XmlOArchive::Private
{
	Private( std::ostream & stream )
		:_Stream( stream )
	{

	}

	bool _Init = false;
	std::ostream & _Stream;
	pugi::xml_document _Document;
	std::deque< pugi::xml_node * > _Stack;
};

XE::XmlOArchive::XmlOArchive( std::ostream & stream )
	:_p( XE::New< Private >( stream ) )
{

}

XE::XmlOArchive::~XmlOArchive()
{
	XE::Delete( _p );
}

void XE::XmlOArchive::Serialize( const XE::Variant & val )
{
	if( !_p->_Init )
	{
		_p->_Init = true;

		auto node = _p->_Document.append_child( pugi::node_declaration );
		node.append_attribute( "version" ).set_value( "1.0" );
		node.append_attribute( "encoding" ).set_value( "utf-8" );
		node.append_attribute( "xe_version" ).set_value( XE_VERSION );

		{
			auto node = _p->_Document.append_child( "xe_object" );

			_p->_Stack.push_back( &node );
			Serialize( val );
			_p->_Stack.pop_back();
		}

		_p->_Document.save( _p->_Stream, PUGIXML_TEXT( "\t" ), pugi::format_default, pugi::xml_encoding::encoding_utf8 );
		_p->_Document = {};

		_p->_Init = false;
	}
	else
	{
		if( val.IsEnum() )
		{
			_p->_Stack.back()->append_attribute( "type" ).set_value( val.GetType()->GetFullName().c_str() );
			_p->_Stack.back()->text().set( SP_CAST< const XE::MetaEnum >( val.GetType() )->FindName( val ).c_str() );
		}
		else if( val.IsFundamental() )
		{
			_p->_Stack.back()->append_attribute( "type" ).set_value( val.GetType()->GetFullName().c_str() );
			switch( val.GetData().index() )
			{
			case 1:
				_p->_Stack.back()->text().set( val.Value< bool >() );
				break;
			case 2:
				_p->_Stack.back()->text().set( val.Value< XE::int8 >() );
				break;
			case 3:
				_p->_Stack.back()->text().set( val.Value< XE::int16 >() );
				break;
			case 4:
				_p->_Stack.back()->text().set( val.Value< XE::int32 >() );
				break;
			case 5:
				_p->_Stack.back()->text().set( val.Value< XE::int64 >() );
				break;
			case 6:
				_p->_Stack.back()->text().set( val.Value< XE::uint8 >() );
				break;
			case 7:
				_p->_Stack.back()->text().set( val.Value< XE::uint16 >() );
				break;
			case 8:
				_p->_Stack.back()->text().set( val.Value< XE::uint32 >() );
				break;
			case 9:
				_p->_Stack.back()->text().set( val.Value< XE::uint64 >() );
				break;
			case 10:
				_p->_Stack.back()->text().set( val.Value< XE::float32 >() );
				break;
			case 11:
				_p->_Stack.back()->text().set( val.Value< XE::float64 >() );
				break;
				break;
			default:
				break;
			}
		}
		else
		{
			if( val.GetType() == ClassID< XE::String >::Get() )
			{
				_p->_Stack.back()->append_attribute( "type" ).set_value( val.GetType()->GetFullName().c_str() );
				_p->_Stack.back()->text().set( val.Value< XE::String >().c_str() );
			}
			else if( val.GetType() == ClassID< XE::ArchiveNameVariant >::Get() )
			{
				auto nvp = val.Value< XE::ArchiveNameVariant >();

				auto node = _p->_Stack.back()->append_child( nvp.Name.c_str() );

				_p->_Stack.push_back( &node );
				Serialize( nvp.Value );
				_p->_Stack.pop_back();
			}
			else
			{
				_p->_Stack.back()->append_attribute( "type" ).set_value( val.GetType()->GetFullName().c_str() );
				_p->_Stack.back()->append_attribute( "flag" ).set_value( val.IsNull() ? flag_str( is_null ) : ( val.IsSharedPtr() ? flag_str( is_shared_ptr ) : flag_str( is_pointer ) ) );

				if( !val.IsNull() )
				{
					auto node = _p->_Stack.back()->append_child( "value" );
					_p->_Stack.push_back( &node );
					val.GetType()->Serialize( *this, val );
					_p->_Stack.pop_back();
				}
			}
		}
	}
}


struct XE::XmlIArchive::Private
{
	Private( std::istream & stream )
	{
		_Document.load( stream );
	}

	bool _Init = false;
	pugi::xml_document _Document;
	std::deque< pugi::xml_node * > _Stack;
};

XE::XmlIArchive::XmlIArchive( std::istream & stream )
	:_p( XE::New< Private >( stream ) )
{

}

XE::XmlIArchive::~XmlIArchive()
{
	XE::Delete( _p );
}

XE::Variant XE::XmlIArchive::Deserialize( const XE::String & name /*= "" */ )
{
	XE::Variant result;

	if( !_p->_Init )
	{
		_p->_Init = true;

		if( _p->_Document.child( "xml" ).attribute( "xe_version" ).as_uint() == XE_VERSION )
		{
			auto node = _p->_Document.child( "xe_object" );
			_p->_Stack.push_back( &node );
			result = Deserialize();
			_p->_Stack.pop_back();
		}

		_p->_Init = false;
	}
	else if( name != "" )
	{
		auto node = _p->_Stack.back()->child( name.c_str() );
		if( !node.empty() )
		{
			_p->_Stack.push_back( &node );
			result = Deserialize();
			_p->_Stack.pop_back();
		}
	}
	else if( XE::MetaTypeCPtr type = XE::Reflection::FindType( _p->_Stack.back()->attribute( "type" ).value() ) )
	{
		if( type == ::TypeID< bool >::Get() )
		{
			result = _p->_Stack.back()->text().as_bool();
		}
		else if( type == ::TypeID< XE::int8 >::Get() )
		{
			result = _p->_Stack.back()->text().as_int();
		}
		else if( type == ::TypeID< XE::int16 >::Get() )
		{
			result = _p->_Stack.back()->text().as_int();
		}
		else if( type == ::TypeID< XE::int32 >::Get() )
		{
			result = _p->_Stack.back()->text().as_int();
		}
		else if( type == ::TypeID< XE::int64 >::Get() )
		{
			result = _p->_Stack.back()->text().as_llong();
		}
		else if( type == ::TypeID< XE::uint8 >::Get() )
		{
			result = _p->_Stack.back()->text().as_uint();
		}
		else if( type == ::TypeID< XE::uint16 >::Get() )
		{
			result = _p->_Stack.back()->text().as_uint();
		}
		else if( type == ::TypeID< XE::uint32 >::Get() )
		{
			result = _p->_Stack.back()->text().as_uint();
		}
		else if( type == ::TypeID< XE::uint64 >::Get() )
		{
			result = _p->_Stack.back()->text().as_ullong();
		}
		else if( type == ::TypeID< XE::float32 >::Get() )
		{
			result = _p->_Stack.back()->text().as_float();
		}
		else if( type == ::TypeID< XE::float64 >::Get() )
		{
			result = _p->_Stack.back()->text().as_double();
		}
		else if( type == ::TypeID< XE::String >::Get() )
		{
			result = XE::String( _p->_Stack.back()->text().as_string() );
		}
		else if( type->GetType() == XE::MetaInfoType::ENUM )
		{
			result = SP_CAST< const XE::MetaEnum >( type )->FindValue( _p->_Stack.back()->text().as_string() );
		}
		else
		{
			auto flag = str_flag( _p->_Stack.back()->attribute( "flag" ).value() );

			if( XE::MetaClassCPtr cls = SP_CAST< const XE::MetaClass >( type ) )
			{
				if( flag == is_null )
				{
					result = XE::VariantData( XE::VariantPointerData( nullptr, type.get() ) );
				}
				else if( flag == is_shared_ptr )
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					cls->Construct( p );
					XE::SharedPtr< void > sp( p, [c = cls.get()]( void * p ) { c->Destruct( p ); XE::MemoryResource::Free( p ); } );
					result = XE::Variant( sp, cls.get() );
				}
				else
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					result = cls->Construct( p );
				}
			}

			if( !result.IsNull() )
			{
				auto node = _p->_Stack.back()->child( "value" );

				_p->_Stack.push_back( &node );
				type->Deserialize( *this, result );
				_p->_Stack.pop_back();
			}
		}
	}

	return result;
}


struct XE::JsonOArchive::Private
{
	Private( std::ostream & stream )
		:_Stream( stream ), _Document( rapidjson::Type::kObjectType )
	{

	}

	bool _Init = false;
	std::ostream & _Stream;
	rapidjson::Document _Document;
	std::deque< rapidjson::Value * > _Stack;
};

XE::JsonOArchive::JsonOArchive( std::ostream & stream )
	:_p( XE::New< Private >( stream ) )
{

}

XE::JsonOArchive::~JsonOArchive()
{
	XE::Delete( _p );
}

void XE::JsonOArchive::Serialize( const XE::Variant & val )
{
	if( !_p->_Init )
	{
		_p->_Init = true;

		_p->_Document.AddMember( rapidjson::StringRef( "xe version" ), rapidjson::Value( XE_VERSION ), _p->_Document.GetAllocator() );

		{
			rapidjson::Value value( rapidjson::kObjectType );

			_p->_Stack.push_back( &value );
			Serialize( val );
			_p->_Stack.pop_back();

			_p->_Document.AddMember( rapidjson::StringRef( "xe object" ), value, _p->_Document.GetAllocator() );
		}

		rapidjson::OStreamWrapper wrapper( _p->_Stream );
		rapidjson::PrettyWriter writer( wrapper );
		_p->_Document.Accept( writer );

		_p->_Document.Parse( "{}" );

		_p->_Init = false;
	}
	else
	{
		if( val.IsEnum() )
		{
			rapidjson::Value type, value;

			type.SetString( val.GetType()->GetFullName().c_str(), _p->_Document.GetAllocator() );
			value.SetString( SP_CAST< const XE::MetaEnum >( val.GetType() )->FindName( val ).c_str(), _p->_Document.GetAllocator() );

			_p->_Stack.back()->AddMember( "type", type, _p->_Document.GetAllocator() );
			_p->_Stack.back()->AddMember( "value", value, _p->_Document.GetAllocator() );
		}
		else if ( val.GetType() == ClassID< bool >::Get() )
		{
			_p->_Stack.back()->SetBool( val.ToBool() );
		}
		else if ( val.IsFundamental() )
		{
			_p->_Stack.back()->SetObject();

			rapidjson::Value key( rapidjson::kStringType );
			rapidjson::Value value( rapidjson::Type::kFalseType );
			key.SetString( val.GetType()->GetFullName().c_str(), _p->_Document.GetAllocator() );

			switch ( val.GetData().index() )
			{
			case 2:
				value.SetInt( val.Value< XE::int8 >() );
				break;
			case 3:
				value.SetInt( val.Value< XE::int16 >() );
				break;
			case 4:
				value.SetInt( val.Value< XE::int32 >() );
				break;
			case 5:
				value.SetInt64( val.Value< XE::int64 >() );
				break;
			case 6:
				value.SetUint( val.Value< XE::uint8 >() );
				break;
			case 7:
				value.SetUint( val.Value< XE::uint16 >() );
				break;
			case 8:
				value.SetUint( val.Value< XE::uint32 >() );
				break;
			case 9:
				value.SetUint64( val.Value< XE::uint64 >() );
				break;
			case 10:
				value.SetFloat( val.Value< XE::float32 >() );
				break;
			case 11:
				value.SetDouble( val.Value< XE::float64 >() );
				break;
			default:
				break;
			}

			_p->_Stack.back()->AddMember( key, value, _p->_Document.GetAllocator() );
		}
		else if ( val.GetType() == ClassID< XE::String >::Get() )
		{
			_p->_Stack.back()->SetString( val.Value< const XE::String & >().c_str(), _p->_Document.GetAllocator() );
		}
		else if ( val.GetType() == ClassID< XE::ArchiveNameVariant >::Get() )
		{
			auto nvp = val.Value< XE::ArchiveNameVariant >();

			rapidjson::Value key( rapidjson::kStringType ), value( rapidjson::kObjectType );

			key.SetString( nvp.Name.c_str(), _p->_Document.GetAllocator() );

			_p->_Stack.push_back( &value );
			Serialize( nvp.Value );
			_p->_Stack.pop_back();

			_p->_Stack.back()->AddMember( key, value, _p->_Document.GetAllocator() );
		}
		else
		{
			rapidjson::Value type( rapidjson::kStringType ), flag( rapidjson::kStringType ), value( rapidjson::kObjectType );

			type.SetString( val.GetType()->GetFullName().c_str(), _p->_Document.GetAllocator() );
			flag.SetString( val.IsNull() ? flag_str( is_null ) : ( val.IsSharedPtr() ? flag_str( is_shared_ptr ) : flag_str( is_pointer ) ), _p->_Document.GetAllocator() );


			_p->_Stack.back()->AddMember( "type", type, _p->_Document.GetAllocator() );
			_p->_Stack.back()->AddMember( "flag", flag, _p->_Document.GetAllocator() );

			if ( val.IsNull() )
			{
				value.SetNull();
			}
			else
			{
				_p->_Stack.push_back( &value );
				val.GetType()->Serialize( *this, val );
				_p->_Stack.pop_back();
			}

			_p->_Stack.back()->AddMember( "value", value, _p->_Document.GetAllocator() );
		}
	}
}


struct XE::JsonIArchive::Private
{
	Private( std::istream & stream )
	{
		rapidjson::IStreamWrapper wrapper( stream );
		_Document.ParseStream( wrapper );
	}

	bool _Init = false;
	rapidjson::Document _Document;
	std::deque< rapidjson::Value * > _Stack;
};

XE::JsonIArchive::JsonIArchive( std::istream & stream )
	:_p( XE::New< Private >( stream ) )
{
}

XE::JsonIArchive::~JsonIArchive()
{
	XE::Delete( _p );
}

XE::Variant XE::JsonIArchive::Deserialize( const XE::String & name /*= ""*/ )
{
	XE::Variant result;

	if( !_p->_Init )
	{
		_p->_Init = true;

		if( _p->_Document["xe version"].GetUint() == XE_VERSION )
		{
			_p->_Stack.push_back( &_p->_Document["xe object"] );
			result = Deserialize();
			_p->_Stack.pop_back();
		}

		_p->_Init = false;
	}
	else if( name != "" )
	{
		if( _p->_Stack.back()->HasMember( name.c_str() ) )
		{
			auto & value = _p->_Stack.back()->FindMember( name.c_str() )->value;

			_p->_Stack.push_back( &value );
			result = Deserialize();
			_p->_Stack.pop_back();
		}
	}
	else if ( _p->_Stack.back()->IsBool() )
	{
		result = _p->_Stack.back()->GetBool();
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::int8 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::int8( _p->_Stack.back()->MemberBegin()->value.GetInt() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::int16 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::int16( _p->_Stack.back()->MemberBegin()->value.GetInt() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::int32 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::int32( _p->_Stack.back()->MemberBegin()->value.GetInt() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::int64 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::int64( _p->_Stack.back()->MemberBegin()->value.GetInt64() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::uint8 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::uint8( _p->_Stack.back()->MemberBegin()->value.GetUint() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::uint16 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::uint16( _p->_Stack.back()->MemberBegin()->value.GetUint() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::uint32 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::uint32( _p->_Stack.back()->MemberBegin()->value.GetUint() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::uint64 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::uint64( _p->_Stack.back()->MemberBegin()->value.GetUint64() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::float32 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::float32( _p->_Stack.back()->MemberBegin()->value.GetFloat() );
	}
	else if ( _p->_Stack.back()->IsObject() && _p->_Stack.back()->HasMember( ::TypeID< XE::float64 >::Get()->GetFullName().c_str() ) )
	{
		result = XE::float64( _p->_Stack.back()->MemberBegin()->value.GetDouble() );
	}
	else if ( _p->_Stack.back()->IsString() )
	{
		result = XE::String( _p->_Stack.back()->GetString() );
	}
	else if ( XE::MetaTypeCPtr type = XE::Reflection::FindType( _p->_Stack.back()->FindMember( "type" )->value.GetString() ) )
	{
		if ( type->GetType() == XE::MetaInfoType::ENUM )
		{
			auto & value = _p->_Stack.back()->FindMember( "value" )->value;
			result = SP_CAST< const XE::MetaEnum >( type )->FindValue( value.GetString() );
		}
		else
		{
			auto flag = str_flag( _p->_Stack.back()->FindMember( "flag" )->value.GetString() );

			if ( XE::MetaClassCPtr cls = SP_CAST< const XE::MetaClass >( type ) )
			{
				if ( flag == is_null )
				{
					result = XE::VariantData( XE::VariantPointerData( nullptr, type.get() ) );
				}
				else if ( flag == is_shared_ptr )
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					cls->Construct( p );
					XE::SharedPtr< void > sp( p, [c = cls.get()]( void * p ) { c->Destruct( p ); XE::MemoryResource::Free( p ); } );
					result = XE::Variant( sp, cls.get() );
				}
				else
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					result = cls->Construct( p );
				}
			}

			if ( !result.IsNull() )
			{
				auto & value = _p->_Stack.back()->FindMember( "value" )->value;

				_p->_Stack.push_back( &value );
				type->Deserialize( *this, result );
				_p->_Stack.pop_back();
			}
		}
	}

	return result;
}


struct XE::BinaryOArchive::Private
{
	Private( std::ostream & stream )
		:_Stream( stream )
	{

	}

	std::ostream & _Stream;
};

XE::BinaryOArchive::BinaryOArchive( std::ostream & stream )
	:_p( XE::New< Private >( stream ) )
{

}

XE::BinaryOArchive::~BinaryOArchive()
{
	XE::Delete( _p );
}

void XE::BinaryOArchive::Serialize( const XE::Variant & val )
{
	write( _p->_Stream, val.GetType()->GetHashCode() );

	if( val.GetType() == ClassID< std::nullptr_t >::Get() )
	{

	}
	else if( val.IsFundamental() )
	{
		if( val.GetType() == TypeID< bool >::Get() )
		{
			write( _p->_Stream, val.Value< bool >() );
		}
		else if( val.GetType() == TypeID< XE::int8 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::int8 >() );
		}
		else if( val.GetType() == TypeID< XE::int16 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::int16 >() );
		}
		else if( val.GetType() == TypeID< XE::int32 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::int32 >() );
		}
		else if( val.GetType() == TypeID< XE::int64 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::int64 >() );
		}
		else if( val.GetType() == TypeID< XE::uint8 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::uint8 >() );
		}
		else if( val.GetType() == TypeID< XE::uint16 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::uint16 >() );
		}
		else if( val.GetType() == TypeID< XE::uint32 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::uint32 >() );
		}
		else if( val.GetType() == TypeID< XE::uint64 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::uint64 >() );
		}
		else if( val.GetType() == TypeID< XE::float32 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::float32 >() );
		}
		else if( val.GetType() == TypeID< XE::float64 >::Get() )
		{
			write( _p->_Stream, val.Value< XE::float64 >() );
		}
	}
	else
	{
		if( val.GetType() == ClassID< XE::String >::Get() )
		{
			write( _p->_Stream, val.Value< const XE::String & >() );
		}
		else if( val.GetType() == ClassID< XE::ArchiveNameVariant >::Get() )
		{
			auto nvp = val.Value< XE::ArchiveNameVariant >();

			Serialize( nvp.Value );
		}
		else if( val.IsEnum() )
		{
			write( _p->_Stream, std::get< XE::VariantEnumData >( val.GetData() ).Value );
		}
		else
		{
			write( _p->_Stream, val.IsNull() ? is_null : ( val.IsSharedPtr() ? is_shared_ptr : is_pointer ) );
			if( !val.IsNull() )
			{
				val.GetType()->Serialize( *this, val );
			}
		}
	}
}


struct XE::BinaryIArchive::Private
{
	Private( std::istream & val )
		:_Stream( val )
	{

	}

	std::istream & _Stream;
};

XE::BinaryIArchive::BinaryIArchive( std::istream & stream )
	: _p( XE::New< Private >( stream ) )
{

}

XE::BinaryIArchive::~BinaryIArchive()
{
	XE::Delete( _p );
}

XE::Variant XE::BinaryIArchive::Deserialize( const XE::String & name /*= ""*/ )
{
	XE::uint64 hash_code = 0;
	read( _p->_Stream, hash_code );

	if( auto type = XE::Reflection::FindType( hash_code ) )
	{
		if( type == ClassID< std::nullptr_t >::Get() )
		{
			return nullptr;
		}
		else if( type == ClassID< bool >::Get() )
		{
			bool val = false;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::int8 >::Get() )
		{
			XE::int8 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::int16 >::Get() )
		{
			XE::int16 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::int32 >::Get() )
		{
			XE::int32 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::int64 >::Get() )
		{
			XE::int64 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::uint8 >::Get() )
		{
			XE::uint8 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::uint16 >::Get() )
		{
			XE::uint16 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::uint32 >::Get() )
		{
			XE::uint32 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::uint64 >::Get() )
		{
			XE::uint64 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::float32 >::Get() )
		{
			XE::float32 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::float64 >::Get() )
		{
			XE::float64 val = 0;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::String >::Get() )
		{
			XE::String val;
			read( _p->_Stream, val );
			return val;
		}
		else if( type == ClassID< XE::ArchiveNameVariant >::Get() )
		{
			return Deserialize();
		}
		else
		{
			if( type->GetType() == XE::MetaInfoType::ENUM )
			{
				XE::uint64 val = 0;
				read( _p->_Stream, val );
				return { XE::VariantData( XE::VariantEnumData( val, type.get() ) ) };
			}
			else if( auto cls = SP_CAST< const XE::MetaClass >( type ) )
			{
				XE::uint8 flag = 0;
				read( _p->_Stream, flag );

				XE::Variant result;

				if( flag == is_null )
				{
					result = XE::VariantData{ XE::VariantPointerData( nullptr, type.get() ) };
				}
				else if( flag == is_shared_ptr )
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					cls->Construct( p );
					XE::SharedPtr< void > sp( p, [c = cls.get()]( void * p ) { c->Destruct( p ); XE::MemoryResource::Free( p ); } );
					result = XE::Variant( sp, cls.get() );
				}
				else
				{
					void * p = XE::MemoryResource::Alloc( cls->GetSize() );
					result = cls->Construct( p );
				}

				if( flag != is_null )
				{
					type->Deserialize( *this, result );
				}

				return result;
			}
		}
	}

	return {};
}
