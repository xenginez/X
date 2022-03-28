#include "BooleanInspector.h"

REG_WIDGET( XS::BooleanInspector );

REG_INSPECTOR( bool, XS::BooleanInspector );

XS::BooleanInspector::BooleanInspector( QWidget * parent /*= nullptr */ )
	:XS::Inspector( parent )
{
	_CheckBox = new QCheckBox( this );
	SetContentWidget( _CheckBox );
}

XS::BooleanInspector::~BooleanInspector()
{

}

void XS::BooleanInspector::Refresh()
{
	disconnect( _CheckBox );

	_CheckBox->setChecked( GetObjectProxy()->GetValue().ToBool() );

	connect( _CheckBox, &QCheckBox::clicked, [this]( bool clicked )
		{
			GetObjectProxy()->SetValue( clicked );
		} );
}
