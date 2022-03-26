#include "BoolInspector.h"

XS::BoolInspector::BoolInspector( QWidget * parent /*= nullptr */ )
	:XS::Inspector( parent )
{
	_CheckBox = new QCheckBox( this );

	connect( _CheckBox, &QCheckBox::clicked, [this]()
		{
			SetVariant( _CheckBox->isChecked() );
		} );
}

XS::BoolInspector::~BoolInspector()
{

}

void XS::BoolInspector::Refresh()
{
	_CheckBox->setChecked( GetVariant().ToBool() );
}
