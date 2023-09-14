#include "MetaClass.h"

#include "MetaMethod.h"
#include "MetaProperty.h"
#include "MetaAttribute.h"

XE::MetaClass::MetaClass( const XE::String & Name, XE::uint64 Size, bool IsAbs, bool IsCont, XE::MetaClassCPtr Super, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module, const XE::TemplateType & temps, const XE::MetaTypeCPtr & element )
	: XE::MetaType( Name, MetaInfoType::CLASS, Size, Owner, Module ), _IsAbstract( IsAbs ), _IsContainer( IsCont ), _Super( Super ), _ElementType( element )
{
	if( Super )
	{
		CP_CAST< XE::MetaClass >( Super )->_DerivedClasses.push_back( this );
	}
}

XE::MetaClass::~MetaClass()
{
	if( auto super = CP_CAST< XE::MetaClass >( _Super.lock() ) )
	{
		auto it = std::find( super->_DerivedClasses.begin(), super->_DerivedClasses.end(), this );
		if( it != super->_DerivedClasses.end() )
		{
			super->_DerivedClasses.erase( it );
		}
	}
}

bool XE::MetaClass::IsAbstract() const
{
	return _IsAbstract;
}

bool XE::MetaClass::IsTemplate() const
{
	return !_Templates.empty();
}

bool XE::MetaClass::IsContainer() const
{
	return _IsContainer;
}

XE::uint64 XE::MetaClass::GetMethodSize() const
{
	return _Methods.size();
}

XE::uint64 XE::MetaClass::GetPropertySize() const
{
	return _Propertys.size();
}

XE::uint64 XE::MetaClass::GetDerivedClassSize() const
{
	return _DerivedClasses.size();
}

XE::MetaTypeCPtr XE::MetaClass::GetElementType() const
{
	return _ElementType;
}

bool XE::MetaClass::CanConvert( const XE::MetaClass * val ) const
{
	auto cls = this;

	do
	{
		if( cls == val )
		{
			return true;
		}

		cls = cls->GetSuper().get();

	} while( cls );

	return false;
}

bool XE::MetaClass::CanConvert( const XE::MetaClassCPtr & val ) const
{
	auto cls = SP_CAST< const MetaClass >( shared_from_this() );

	do
	{
		if( cls == val )
		{
			return true;
		}

		cls = cls->GetSuper();

	} while( cls );

	return false;
}

XE::MetaClassCPtr XE::MetaClass::GetSuper() const
{
	return _Super.lock();
}

const XE::TemplateType & XE::MetaClass::GetTemplateTypes() const
{
	return _Templates;
}

const XE::Array< XE::MetaMethodCPtr > & XE::MetaClass::GetMethods() const
{
	return _Methods;
}

const XE::Array< XE::MetaPropertyCPtr > & XE::MetaClass::GetPropertys() const
{
	return _Propertys;
}

const XE::Array< const XE::MetaClass * > & XE::MetaClass::GetDerivedClasses() const
{
	return _DerivedClasses;
}

void XE::MetaClass::VisitMethod( const XE::Delegate< void( const XE::MetaMethodCPtr & ) > & val ) const
{
	for( auto var : _Methods )
	{
		val( var );
	}
	if( auto super = _Super.lock() )
	{
		super->VisitMethod( val );
	}
}

void XE::MetaClass::VisitProperty( const XE::Delegate< void( const XE::MetaPropertyCPtr & ) > & val ) const
{
	for( auto var : _Propertys )
	{
		val( var );
	}
	if( auto super = _Super.lock() )
	{
		super->VisitProperty( val );
	}
}

void XE::MetaClass::VisitDerivedClass( const XE::Delegate< void( const XE::MetaClassCPtr & ) > & val ) const
{
	for( const auto & var : _DerivedClasses )
	{
		val( SP_CAST< const XE::MetaClass >( var->shared_from_this() ) );
	}
}

XE::MetaMethodCPtr XE::MetaClass::FindMethod( XE::uint64 hash ) const
{
	for( auto var : _Methods )
	{
		if( var->GetHashCode() == hash )
		{
			return var;
		}
	}

	if( auto super = _Super.lock() )
	{
		return super->FindMethod( hash );
	}

	return nullptr;
}

XE::MetaPropertyCPtr XE::MetaClass::FindProperty( XE::uint64 hash ) const
{
	for( auto var : _Propertys )
	{
		if( var->GetHashCode() == hash )
		{
			return var;
		}
	}

	if( auto super = _Super.lock() )
	{
		return super->FindProperty( hash );
	}

	return nullptr;
}

XE::MetaMethodCPtr XE::MetaClass::FindMethod( const XE::String & Name ) const
{
	for( auto var : _Methods )
	{
		if( var->GetName() == Name )
		{
			return var;
		}
	}

	if( auto super = _Super.lock() )
	{
		return super->FindMethod( Name );
	}

	return nullptr;
}

XE::MetaMethodCPtr XE::MetaClass::FindMethod( const XE::String & Name, const XE::ParameterType & Types /*= XE::MakeParameterType() */ ) const
{
	return FindMethod( Name + XE::ToString( Types ) );
}

XE::MetaPropertyCPtr XE::MetaClass::FindProperty( const XE::String & Name ) const
{
	for( auto var : _Propertys )
	{
		if( var->GetName() == Name )
		{
			return var;
		}
	}

	if( auto super = _Super.lock() )
	{
		return super->FindProperty( Name );
	}

	return nullptr;
}

void XE::MetaClass::_RegisterMethod( const XE::MetaMethodPtr & val )
{
	_Methods.push_back( val );
}

void XE::MetaClass::_RegisterProperty( const XE::MetaPropertyPtr & val )
{
	_Propertys.push_back( val );
}
