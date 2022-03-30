#include "ReflectObject.h"

#include "CXXMetaClass.hpp"

XE::ReflectObject::ReflectObject()
{
}

XE::ReflectObject::~ReflectObject()
{
}

XE::MetaClassCPtr XE::ReflectObject::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::ReflectObject > >( "ReflectObject", nullptr, nullptr, GetModule() );
	return p;
}

XE::MetaClassCPtr XE::ReflectObject::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::Variant XE::ReflectObject::Get( const String & name )
{
	return GetMetaClass()->FindProperty( name )->Get( this );
}

void XE::ReflectObject::Set( const String & name, const Variant & val )
{
	GetMetaClass()->FindProperty( name )->Set( this, val );
}

XE::Variant XE::ReflectObject::Invoke( const String & name, InvokeStack * params )
{
	if( auto method = GetMetaClass()->FindMethod( name, params->GetTypes() ) )
	{
		return method->Invoke( params );
	}

	return {};
}

void XE::ReflectObject::Serialize( XE::OArchive & archive ) const
{
	GetMetaClass()->VisitProperty( [&]( const XE::MetaPropertyCPtr & prop )
						{
							if( !prop->IsStatic() && prop->FindAttributeT< XE::NonSerializeAttribute >() == nullptr )
							{
								archive << ARCHIVE_NVP( prop->GetName(), prop->Get( this ) );
							}
						} );
}

void XE::ReflectObject::Deserialize( XE::IArchive & archive )
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
