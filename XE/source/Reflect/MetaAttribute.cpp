#include "MetaAttribute.h"

#include "Reflection.h"

XE::MetaClassPtr XE::MetaAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::MetaAttribute > >( "MetaAttribute", nullptr, nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::MetaAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::FlagAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::FlagAttribute > >( "FlagAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::FlagAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::LinkAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::LinkAttribute > >( "LinkAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::LinkAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::LinkAttribute::LinkAttribute( const XE::String & name, const XE::Delegate<bool( const XE::Variant & link )> & callback )
	:_Name( name ), _Callback( callback )
{

}

const XE::String & XE::LinkAttribute::GetLinkName() const
{
	return _Name;
}

const XE::Delegate<bool( const XE::Variant & ) > & XE::LinkAttribute::GetLinkCallback() const
{
	return _Callback;
}


XE::RangeAttribute::RangeAttribute( XE::float64 min, XE::float64 max, XE::float64 step /*= 1.0 */ )
	:_Min( min ), _Max( max ), _Step( step )
{
}

XE::MetaClassPtr XE::RangeAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::RangeAttribute > >( "RangeAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::RangeAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::float64 XE::RangeAttribute::GetMin() const
{
	return _Min;
}

XE::float64 XE::RangeAttribute::GetMax() const
{
	return _Max;
}

XE::float64 XE::RangeAttribute::GetStep() const
{
	return _Step;
}


XE::MetaClassPtr XE::NonCloneAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonCloneAttribute > >( "NonCloneAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonCloneAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::InspectorAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::InspectorAttribute > >( "InspectorAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::InspectorAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::InspectorAttribute::InspectorAttribute( const XE::String & type )
	:_Type( type )
{

}

const XE::String & XE::InspectorAttribute::GetTypeName() const
{
	return _Type;
}


XE::MetaClassPtr XE::NonInspectorAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonInspectorAttribute > >( "NonInspectorAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonInspectorAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonSerializeAttribute > >( "NonSerializeAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::ParameterNamesAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::ParameterNamesAttribute > >( "ParameterNamesAttribute", XE::MetaAttribute::GetMetaClassStatic(), nullptr, XE::GetModule() );
	return p;
}

XE::MetaClassPtr XE::ParameterNamesAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}

XE::ParameterNamesAttribute::ParameterNamesAttribute( const XE::Array<XE::String> & names )
	: _Names( names )
{
}

const XE::Array<XE::String> & XE::ParameterNamesAttribute::GetNames() const
{
	return _Names;
}
