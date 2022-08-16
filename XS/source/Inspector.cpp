#include "Inspector.h"

#include <QMap>
#include <QVBoxLayout>

namespace
{
	QMap< XE::MetaTypeCPtr, QString > _Registers = {};
}

XS::Inspector::Inspector( QWidget * parent /*= nullptr*/ )
	:XS::Widget( parent )
{

}

XS::Inspector::~Inspector()
{
	if ( _Proxy )
	{
		delete _Proxy;
	}
}

void XS::Inspector::Register( const XE::MetaTypeCPtr & type, const QString & name )
{
	_Registers.insert( type, name );
}

XS::Inspector * XS::Inspector::Create( XS::ObjectProxy * proxy, QWidget * parent /*= nullptr */ )
{
	XS::Inspector * result = nullptr;

	auto type = proxy->GetType();
	if ( auto attr = type->FindAttributeT< XE::InspectorAttribute >() )
	{
		result = XS::Registry::ConstructT< XS::Inspector >( attr->GetTypeName().c_str(), parent );
	}
	else
	{
		auto it = _Registers.find( type );
		if ( it != _Registers.end() )
		{
			result = XS::Registry::ConstructT< XS::Inspector >( it.value(), parent );
		}
		else if ( type->GetType() == XE::MetaInfoType::ENUM )
		{
			if ( proxy->FindAttributeT< XE::FlagAttribute >() != nullptr )
			{
				result = XS::Registry::ConstructT< XS::Inspector >( "XS::FlagInspector", parent );//new XS::FlagInspector( parent );
			}
			else
			{
			 	result = XS::Registry::ConstructT< XS::Inspector >( "XS::EnumInspector", parent );//new XS::EnumInspector( parent );
			}
		}
		else if ( proxy->GetType()->IsContainer() )
		{
			if ( proxy->GetType()->GetFullName().find_first_of( "XE::Pair" ) == 0 )
			{
				result = XS::Registry::ConstructT< XS::Inspector >( "XS::PairInspector", parent );//new XS::PairInspector( parent );
			}
			else
			{
				result = XS::Registry::ConstructT< XS::Inspector >( "XS::ContainerInspector", parent );//new XS::ContainerInspector( parent );
			}
		}
		else
		{
			result = XS::Registry::ConstructT< XS::Inspector >( "XS::ClassInspector", parent );//new XS::ClassInspector( parent );
		}
	}

	result->SetObjecrProxy( proxy );

	result->Refresh();

	return result;
}

void XS::Inspector::Expand()
{

}

void XS::Inspector::Collapse()
{

}

XS::ObjectProxy * XS::Inspector::GetObjectProxy()
{
	return _Proxy;
}

void XS::Inspector::SetObjecrProxy( XS::ObjectProxy * proxy )
{
	_Proxy = proxy;
}

void XS::Inspector::SetContentWidget( QWidget * widget )
{
	QVBoxLayout * verticalLayout = new QVBoxLayout( this );

	verticalLayout->setSpacing( 0 );
	verticalLayout->setObjectName( QString::fromUtf8( "layout" ) );
	verticalLayout->setContentsMargins( 0, 0, 0, 0 );

	verticalLayout->addWidget( widget );
}

XS::GroupInspector::GroupInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{

}

XS::GroupInspector::~GroupInspector()
{

}
