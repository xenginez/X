#include "Inspector.h"

#include <QMap>
#include <QVBoxLayout>

#include "Inspectors/EnumInspector.h"
#include "Inspectors/FlagInspector.h"
#include "Inspectors/ClassInspector.h"

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

}

void XS::Inspector::Register( const XE::MetaTypeCPtr & type, const QString & name )
{
	_Registers.insert( type, name );
}

XS::Inspector * XS::Inspector::Create( const XS::ObjectProxyPtr & proxy, QWidget * parent /*= nullptr */ )
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
				result = new XS::FlagInspector( parent );
			}
			else
			{
			 	result = new XS::EnumInspector( parent );
			}
		}
		else
		{
			result = new XS::ClassInspector( parent );
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

const XS::ObjectProxyPtr & XS::Inspector::GetObjectProxy()
{
	return _Proxy;
}

void XS::Inspector::SetObjecrProxy( const XS::ObjectProxyPtr & proxy )
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
