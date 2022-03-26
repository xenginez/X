#include "MetaType.h"

#include "MetaAttribute.h"

XE::MetaType::MetaType( const String & Name, MetaInfoType Type, XE::uint64 Size, MetaInfoPtr Owner, MetaModulePtr Module )
	:MetaInfo( Name, Type, Owner, Module ), _Size( Size )
{

}


XE::MetaType::~MetaType()
{

}

XE::uint64 XE::MetaType::GetSize() const
{
	return _Size;
}

XE::MetaAttributeCPtr XE::MetaType::FindAttribute( const XE::MetaClassPtr & val ) const
{
	for ( const auto & it : _Attributes )
	{
		if ( it->GetMetaClass() == val )
		{
			return it;
		}
	}

	return nullptr;
}

const XE::Array< XE::MetaAttributeCPtr > & XE::MetaType::GetAttributes() const
{
	return _Attributes;
}
