#include "BoolInspector.h"

REG_WIDGET( XS::BoolInspector );

REG_INSPECTOR( bool, XS::BoolInspector );

XS::BoolInspector::BoolInspector( QWidget * parent /*= nullptr */ )
	:XS::Inspector( parent )
{
	_CheckBox = new QCheckBox( this );

	connect( _CheckBox, &QCheckBox::clicked, [this]( bool clicked )
		{
			GetObjectProxy().SetValue( clicked );
		} );
}

XS::BoolInspector::~BoolInspector()
{

}

void XS::BoolInspector::Refresh()
{
	_CheckBox->setChecked( GetObjectProxy().GetValue().ToBool() );
}
