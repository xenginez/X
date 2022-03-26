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

XE::Variant XE::MetaAttribute::Execute( const XE::Variant & val ) const
{
	return val;
}


XE::RangeAttribute::RangeAttribute( XE::float64 min, XE::float64 max )
	:_Min( min ), _Max( max )
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

XE::Variant XE::RangeAttribute::Execute( const XE::Variant & val ) const
{
	return std::clamp( val.ToFloat64(), _Min, _Max );
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


XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClassStatic()
{
	static auto p = XE::MakeShared< XE::CXXMetaClass< XE::NonSerializeAttribute > >( "NonSerializeAttribute", MetaAttribute::GetMetaClassStatic(), nullptr, GetModule() );
	return p;
}

XE::MetaClassPtr XE::NonSerializeAttribute::GetMetaClass() const
{
	return GetMetaClassStatic();
}
