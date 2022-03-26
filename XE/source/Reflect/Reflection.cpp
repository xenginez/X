#include "Reflection.h"

#include "MetaEnum.h"
#include "MetaClass.h"
#include "MetaModule.h"
#include "MetaMethod.h"
#include "MetaProperty.h"
#include "MetaFunction.h"
#include "MetaVariable.h"

struct XE::Reflection::Private
{
	XE::Array< XE::MetaEnumPtr >		_Enums;
	XE::Array< XE::MetaClassPtr >		_Classs;
	XE::Array< XE::MetaModulePtr >		_Modules;
	XE::Array< XE::MetaMethodPtr >		_Methods;
	XE::Array< XE::MetaPropertyPtr >	_Propertys;
	XE::Array< XE::MetaFunctionPtr >	_Functions;
	XE::Array< XE::MetaVariablePtr >	_Variables;


	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashEnum;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashClass;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashModule;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashMethod;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashProperty;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashFunction;
	XE::UnorderedMap< XE::uint64, XE::uint64 > _HashVariable;

	XE::UnorderedMap< XE::String, XE::uint64 > _NameEnum;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameClass;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameModule;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameMethod;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameProperty;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameFunction;
	XE::UnorderedMap< XE::String, XE::uint64 > _NameVariable;
};

XE::Reflection::Reflection()
	:_p( XE::New< Private >() )
{

}

XE::Reflection::~Reflection()
{
	XE::Delete( _p );
}

XE::Reflection * XE::Reflection::Instance()
{
	static XE::Reflection instance;
	return &instance;
}

#define _p XE::Reflection::Instance()->_p

void XE::Reflection::RegisterMetaInfo( XE::MetaInfoPtr val )
{
	switch( val->GetType() )
	{
	case XE::MetaInfoType::ENUM:
	{
		_p->_Enums.push_back( SP_CAST< XE::MetaEnum >( val ) );
		auto index = _p->_Enums.size() - 1;
		_p->_HashEnum.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameEnum.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::CLASS:
	{
		_p->_Classs.push_back( SP_CAST< XE::MetaClass >( val ) );
		auto index = _p->_Classs.size() - 1;
		_p->_HashClass.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameClass.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::MODULE:
	{
		_p->_Modules.push_back( SP_CAST< XE::MetaModule >( val ) );
		auto index = _p->_Modules.size() - 1;
		_p->_HashModule.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameModule.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::METHOD:
	{
		_p->_Methods.push_back( SP_CAST< XE::MetaMethod >( val ) );
		auto index = _p->_Methods.size() - 1;
		_p->_HashMethod.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameMethod.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::PROPERTY:
	{
		_p->_Propertys.push_back( SP_CAST< XE::MetaProperty >( val ) );
		auto index = _p->_Propertys.size() - 1;
		_p->_HashProperty.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameProperty.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::FUNCTION:
	{
		_p->_Functions.push_back( SP_CAST< XE::MetaFunction >( val ) );
		auto index = _p->_Functions.size() - 1;
		_p->_HashFunction.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameFunction.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	case XE::MetaInfoType::VARIABLE:
	{
		_p->_Variables.push_back( SP_CAST< XE::MetaVariable >( val ) );
		auto index = _p->_Variables.size() - 1;
		_p->_HashVariable.insert( std::make_pair( val->GetHashCode(), index ) );
		_p->_NameVariable.insert( std::make_pair( val->GetFullName(), index ) );
		break;
	}
	default:
		break;
	}
}

void XE::Reflection::VisitMeta( const XE::Delegate< void( const XE::MetaInfoPtr & ) > & val )
{
	VisitEnum( [&]( const XE::MetaEnumPtr & it )
			   {
				   val( it );
			   } );

	VisitClass( [&]( const XE::MetaClassPtr & it )
				{
					val( it );
				} );

	VisitModule( [&]( const XE::MetaModulePtr & it )
				 {
					 val( it );
				 } );

	VisitMethod( [&]( const XE::MetaMethodPtr & it )
				 {
					 val( it );
				 } );

	VisitProperty( [&]( const XE::MetaPropertyPtr & it )
				   {
					   val( it );
				   } );
}

void XE::Reflection::VisitType( const XE::Delegate< void( const XE::MetaTypePtr & ) > & val )
{
	VisitEnum( [&]( const XE::MetaEnumPtr & enm )
			   {
				   val( enm );
			   } );

	VisitClass( [&]( const XE::MetaClassPtr & cls )
				{
					val( cls );
				} );
}

void XE::Reflection::VisitEnum( const XE::Delegate< void( const XE::MetaEnumPtr & ) > & val )
{
	for( auto it : _p->_Enums )
	{
		val( it );
	}
}

void XE::Reflection::VisitClass( const XE::Delegate< void( const XE::MetaClassPtr & ) > & val )
{
	for( auto it : _p->_Classs )
	{
		val( it );
	}
}

void XE::Reflection::VisitModule( const XE::Delegate< void( const XE::MetaModulePtr & ) > & val )
{
	for( auto it : _p->_Modules )
	{
		val( it );
	}
}

void XE::Reflection::VisitMethod( const XE::Delegate< void( const XE::MetaMethodPtr & ) > & val )
{
	for( auto it : _p->_Methods )
	{
		val( it );
	}
}

void XE::Reflection::VisitProperty( const XE::Delegate< void( const  XE::MetaPropertyPtr & ) > & val )
{
	for( auto it : _p->_Propertys )
	{
		val( it );
	}
}

void XE::Reflection::VisitFunction( const XE::Delegate< void( const XE::MetaFunctionPtr & ) > & val )
{
	for( auto it : _p->_Functions )
	{
		val( it );
	}
}

void XE::Reflection::VisitVariable( const XE::Delegate< void( const XE::MetaVariablePtr & ) > & val )
{
	for( auto it : _p->_Variables )
	{
		val( it );
	}
}

XE::MetaInfoPtr XE::Reflection::FindMeta( XE::uint64 HashCode )
{
	if( auto result = FindEnum( HashCode ) )
	{
		return result;
	}

	if( auto result = FindClass( HashCode ) )
	{
		return result;
	}

	if( auto result = FindMethod( HashCode ) )
	{
		return result;
	}

	if( auto result = FindProperty( HashCode ) )
	{
		return result;
	}

	return nullptr;
}

XE::MetaTypePtr XE::Reflection::FindType( XE::uint64 HashCode )
{
	if( auto result = FindEnum( HashCode ) )
	{
		return result;
	}

	if( auto result = FindClass( HashCode ) )
	{
		return result;
	}

	return nullptr;
}

XE::MetaEnumPtr XE::Reflection::FindEnum( XE::uint64 HashCode )
{
	auto it = _p->_HashEnum.find( HashCode );

	if( it != _p->_HashEnum.end() )
	{
		return _p->_Enums[it->second];
	}

	return nullptr;
}

XE::MetaClassPtr XE::Reflection::FindClass( XE::uint64 HashCode )
{
	auto it = _p->_HashClass.find( HashCode );

	if( it != _p->_HashClass.end() )
	{
		return _p->_Classs[it->second];
	}

	return nullptr;
}

XE::MetaModulePtr XE::Reflection::FindModule( XE::uint64 HashCode )
{
	auto it = _p->_HashModule.find( HashCode );

	if( it != _p->_HashModule.end() )
	{
		return _p->_Modules[it->second];
	}

	return nullptr;
}

XE::MetaMethodPtr XE::Reflection::FindMethod( XE::uint64 HashCode )
{
	auto it = _p->_HashMethod.find( HashCode );

	if( it != _p->_HashMethod.end() )
	{
		return _p->_Methods[it->second];
	}

	return nullptr;
}

XE::MetaPropertyPtr XE::Reflection::FindProperty( XE::uint64 HashCode )
{
	auto it = _p->_HashProperty.find( HashCode );

	if( it != _p->_HashProperty.end() )
	{
		return _p->_Propertys[it->second];
	}

	return nullptr;
}

XE::MetaFunctionPtr XE::Reflection::FindFunction( XE::uint64 HashCode )
{
	auto it = _p->_HashFunction.find( HashCode );

	if( it != _p->_HashFunction.end() )
	{
		return _p->_Functions[it->second];
	}

	return nullptr;
}

XE::MetaVariablePtr XE::Reflection::FindVariable( XE::uint64 HashCode )
{
	auto it = _p->_HashVariable.find( HashCode );

	if( it != _p->_HashVariable.end() )
	{
		return _p->_Variables[it->second];
	}

	return nullptr;
}


XE::MetaInfoPtr XE::Reflection::FindMeta( const XE::String & FullName )
{
	if( auto ret = FindEnum( FullName ) )
	{
		return ret;
	}

	if( auto ret = FindClass( FullName ) )
	{
		return ret;
	}

	if( auto ret = FindMethod( FullName ) )
	{
		return ret;
	}

	if( auto ret = FindProperty( FullName ) )
	{
		return ret;
	}

	return nullptr;
}

XE::MetaTypePtr XE::Reflection::FindType( const XE::String & FullName )
{
	if( auto ret = FindEnum( FullName ) )
	{
		return ret;
	}

	if( auto ret = FindClass( FullName ) )
	{
		return ret;
	}

	return nullptr;
}

XE::MetaEnumPtr XE::Reflection::FindEnum( const XE::String & FullName )
{
	auto it = _p->_NameEnum.find( FullName );

	if( it != _p->_NameEnum.end() )
	{
		return _p->_Enums[it->second];
	}

	return nullptr;
}

XE::MetaClassPtr XE::Reflection::FindClass( const XE::String & FullName )
{
	auto it = _p->_NameClass.find( FullName );

	if( it != _p->_NameClass.end() )
	{
		return _p->_Classs[it->second];
	}

	return nullptr;
}

XE::MetaModulePtr XE::Reflection::FindModule( const XE::String & FullName )
{
	auto it = _p->_NameModule.find( FullName );

	if( it != _p->_NameModule.end() )
	{
		return _p->_Modules[it->second];
	}

	return nullptr;
}

XE::MetaMethodPtr XE::Reflection::FindMethod( const XE::String & FullName )
{
	auto it = _p->_NameMethod.find( FullName );

	if( it != _p->_NameMethod.end() )
	{
		return _p->_Methods[it->second];
	}

	return nullptr;
}

XE::MetaPropertyPtr XE::Reflection::FindProperty( const XE::String & FullName )
{
	auto it = _p->_NameProperty.find( FullName );

	if( it != _p->_NameProperty.end() )
	{
		return _p->_Propertys[it->second];
	}

	return nullptr;
}

XE::MetaFunctionPtr XE::Reflection::FindFunction( const XE::String & FullName )
{
	auto it = _p->_NameFunction.find( FullName );

	if( it != _p->_NameFunction.end() )
	{
		return _p->_Functions[it->second];
	}

	return nullptr;
}

XE::MetaVariablePtr XE::Reflection::FindVariable( const XE::String & FullName )
{
	auto it = _p->_NameVariable.find( FullName );

	if( it != _p->_NameVariable.end() )
	{
		return _p->_Variables[it->second];
	}

	return nullptr;
}
