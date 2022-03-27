#include "StringInspector.h"

REG_WIDGET( XS::StringInspector );

REG_INSPECTOR( XE::String, XS::StringInspector );

XS::StringInspector::StringInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QLineEdit = new QLineEdit( this );

	connect( _QLineEdit, &QLineEdit::textChanged, [this]( const QString & text )
		{
			GetObjectProxy().SetValue( XE::String( text.toStdString() ) );
		} );
}

XS::StringInspector::~StringInspector()
{

}

void XS::StringInspector::Refresh()
{
	_QLineEdit->setText( GetObjectProxy().GetValue().Value<XE::String>().c_str() );
}
