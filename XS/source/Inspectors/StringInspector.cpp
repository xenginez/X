#include "StringInspector.h"

REG_WIDGET( XS::StringInspector );

REG_INSPECTOR( XE::String, XS::StringInspector );

XS::StringInspector::StringInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QLineEdit = new QLineEdit( this );
	SetContentWidget( _QLineEdit );
}

XS::StringInspector::~StringInspector()
{

}

void XS::StringInspector::Refresh()
{
	disconnect( _QLineEdit, nullptr );

	_QLineEdit->setText( GetObjectProxy()->GetValue().Value<XE::String>().c_str() );

	connect( _QLineEdit, &QLineEdit::textChanged, [this]( const QString & text )
		{
			GetObjectProxy()->SetValue( XE::String( text.toStdString() ) );
		} );
}
