#include "MetaAttribute.h"

#include "Reflection.h"

XE::MetaClassPtr XE::MetaAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::MetaAttribute > >( "MetaAttribute", nullptr, nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::MetaAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::FlagAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::FlagAttribute > >( "FlagAttribute", nullptr, nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::FlagAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::RangeAttribute::RangeAttribute( XE::float64 min, XE::float64 max, XE::float64 step /*= 1.0 */ )
	:_Min( min ), _Max( max ), _Step( step )
{
}

XE::MetaClassPtr XE::RangeAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::RangeAttribute > >( "RangeAttribute", nullptr, nullptr, GetModule() );
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
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonCloneAttribute > >( "NonCloneAttribute", MetaAttribute::GetMetaClassStatic(), nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonCloneAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::NonEditorAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonEditorAttribute > >( "NonEditorAttribute", MetaAttribute::GetMetaClassStatic(), nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonEditorAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}


XE::MetaClassPtr XE::InspectorAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::InspectorAttribute > >( "InspectorAttribute", MetaAttribute::GetMetaClassStatic(), nullptr, GetModule() );
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


XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonSerializeAttribute > >( "NonSerializeAttribute", MetaAttribute::GetMetaClassStatic(), nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}
