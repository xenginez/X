#include "MetaInfo.h"

#include "Reflection.h"
#include "MetaAttribute.h"

XE::MetaInfo::MetaInfo( const XE::String & Name, XE::MetaInfoType Type, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module )
	: _Type( Type ), _Name( Name ), _FullName( Name ), _Owner( Owner ), _Module( Module )
{
	if( Owner )
	{
		_FullName = Owner->GetFullName() + "." + _Name;
	}
	else if( Module )
	{
		_FullName = Module->GetFullName() + "." + _Name;
	}
	else
	{
		_FullName = _Name;
	}

	_HashCode = XE::Hash( _FullName );
}

XE::MetaInfo::~MetaInfo()
{

}

XE::MetaInfoType XE::MetaInfo::GetType() const
{
	return _Type;
}

XE::uint64 XE::MetaInfo::GetHashCode() const
{
	return _HashCode;
}

const XE::String & XE::MetaInfo::GetName() const
{
	return _Name;
}

const XE::String & XE::MetaInfo::GetFullName() const
{
	return _FullName;
}

const XE::Array< XE::MetaAttributeCPtr > & XE::MetaInfo::GetAttributes() const
{
	return _Attributes;
}

XE::MetaInfoCPtr XE::MetaInfo::GetOwner() const
{
	return _Owner.lock();
}

XE::MetaModuleCPtr XE::MetaInfo::GetModule() const
{
	return _Module.lock();
}

XE::MetaAttributeCPtr XE::MetaInfo::FindAttribute( const XE::MetaClassCPtr & val ) const
{
	for( const auto & it : _Attributes )
	{
		if( it->GetMetaClass() == val )
		{
			return it;
		}
	}

	return nullptr;
}

void XE::MetaInfo::Register()
{
	if( ( _Type == XE::MetaInfoType::ENUM || _Type == XE::MetaInfoType::CLASS ) && _Module.lock() )
	{
		CP_CAST< XE::MetaModule >( _Module.lock() )->RegisterMetaInfo( shared_from_this() );
	}

	XE::Reflection::RegisterMetaInfo( shared_from_this() );
}

XE::Pair< bool, XE::MetaInfo * > XE::MetaInfo::AllocMetaInfo( XE::uint64 code, XE::uint64 size )
{
	static XE::UnorderedMap< XE::uint64, XE::MetaInfo * > maps;

	if( code == 0 )
	{
		return { false,  static_cast< XE::MetaInfo * >( ::malloc( size ) ) };
	}

	auto it = maps.find( code );
	if( it == maps.end() )
	{
		auto p = static_cast< XE::MetaInfo * >( ::malloc( size ) );
		maps.insert( { code, p } );
		return { false, p };
	}

	return { true, it->second };
}
