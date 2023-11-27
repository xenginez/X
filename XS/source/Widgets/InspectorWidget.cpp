#include "InspectorWidget.h"

#include <QMap>
#include <QVBoxLayout>

namespace
{
	QMap< XE::String, QString > & _TemplateRegisters()
	{
		static QMap< XE::String, QString > map;
		return map;
	}
	QMap< XE::MetaTypeCPtr, QString > & _Registers()
	{
		static QMap< XE::MetaTypeCPtr, QString > map;
		return map;
	}
}

XS::InspectorWidget::InspectorWidget( QWidget * parent /*= nullptr*/ )
	:XS::Widget( parent )
{

}

XS::InspectorWidget::~InspectorWidget()
{
	if ( _Proxy )
	{
		delete _Proxy;
	}
}

void XS::InspectorWidget::Register( const XE::String & tname, const QString & name )
{
	_TemplateRegisters().insert(tname, name);
}

void XS::InspectorWidget::Register( const XE::MetaTypeCPtr & type, const QString & name )
{
	_Registers().insert(type, name);
}

XS::InspectorWidget * XS::InspectorWidget::Create( XS::ObjectProxy * proxy, QWidget * parent /*= nullptr */ )
{
	XS::InspectorWidget * result = nullptr;

	auto type = proxy->GetType();
	if ( auto attr = type->FindAttributeT< XE::InspectorAttribute >() )
	{
		result = XS::Registry::ConstructT< XS::InspectorWidget >( attr->GetTypeName().c_str(), parent );
	}
	else
	{
		auto it = _Registers().find(type);
		if ( it != _Registers().end() )
		{
			result = XS::Registry::ConstructT< XS::InspectorWidget >( it.value(), parent );
		}
		else if ( type->GetType() == XE::MetaInfoType::ENUM )
		{
			if ( proxy->FindAttributeT< XE::FlagAttribute >() != nullptr )
			{
				result = XS::Registry::ConstructT< XS::InspectorWidget >( "XS::FlagInspector", parent );//new XS::FlagInspector( parent );
			}
			else
			{
			 	result = XS::Registry::ConstructT< XS::InspectorWidget >( "XS::EnumInspector", parent );//new XS::EnumInspector( parent );
			}
		}
		else if ( auto cls = SP_CAST< const XE::MetaClass >( type ) )
		{
			if ( cls->IsContainer() )
			{
				result = XS::Registry::ConstructT< XS::InspectorWidget >( "XS::ContainerInspector", parent );//new XS::ContainerInspector( parent );
			}
			else if ( cls->IsTemplate() )
			{
				auto name = cls->GetFullName().substr( 0, cls->GetFullName().find( "<" ) );
				auto it = _TemplateRegisters().find(name);
				if ( it != _TemplateRegisters().end() )
				{
					result = XS::Registry::ConstructT< XS::InspectorWidget >( it.value(), parent );
				}
			}
			else
			{
				result = XS::Registry::ConstructT< XS::InspectorWidget >( "XS::ClassInspector", parent );//new XS::ClassInspector( parent );
			}
		}
	}

	result->SetObjecrProxy( proxy );

	result->Refresh();

	return result;
}

void XS::InspectorWidget::Expand()
{

}

void XS::InspectorWidget::Collapse()
{

}

XS::ObjectProxy * XS::InspectorWidget::GetObjectProxy()
{
	return _Proxy;
}

void XS::InspectorWidget::SetObjecrProxy( XS::ObjectProxy * proxy )
{
	_Proxy = proxy;
}

void XS::InspectorWidget::SetContentWidget( QWidget * widget )
{
	QVBoxLayout * verticalLayout = new QVBoxLayout( this );

	verticalLayout->setSpacing( 0 );
	verticalLayout->setObjectName( QString::fromUtf8( "layout" ) );
	verticalLayout->setContentsMargins( 0, 0, 0, 0 );

	verticalLayout->addWidget( widget );
}

XS::GroupInspector::GroupInspector( QWidget * parent /*= nullptr */ )
	:InspectorWidget( parent )
{

}

XS::GroupInspector::~GroupInspector()
{

}
