#include "Object.h"

#include "CXXMetaClass.hpp"

XE::Object::Object()
{
}

XE::Object::~Object()
{
}

XE::MetaClassCPtr XE::Object::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::Object > >( "ReflectObject", nullptr, nullptr, GetModule() );
	return p;
}

XE::MetaClassCPtr XE::Object::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::Variant XE::Object::Get( const String & name )
{
	return GetMetaClass()->FindProperty( name )->Get( this );
}

void XE::Object::Set( const String & name, const Variant & val )
{
	GetMetaClass()->FindProperty( name )->Set( this, val );
}

XE::Variant XE::Object::Invoke( const String & name, InvokeStack * params )
{
	if( auto method = GetMetaClass()->FindMethod( name, params->GetTypes() ) )
	{
		return method->Invoke( params );
	}

	return {};
}

void XE::Object::Serialize( XE::OArchive & archive ) const
{
	GetMetaClass()->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
						{
							if( !prop->IsStatic() && prop->FindAttributeT< XE::NonSerializeAttribute >() == nullptr )
							{
								archive << ARCHIVE_NVP( prop->GetName(), prop->Get( this ) );
							}
						} );
}

void XE::Object::Deserialize( XE::IArchive & archive )
{
	GetMetaClass()->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
						{
							if( !prop->IsStatic() && prop->FindAttributeT< XE::NonSerializeAttribute >() == nullptr )
							{
								XE::Variant var = prop->Get( this );

								auto nvp = ARCHIVE_NVP( prop->GetName(), var );

								archive >> nvp;

								prop->Set( this, var );
							}
						} );
}
