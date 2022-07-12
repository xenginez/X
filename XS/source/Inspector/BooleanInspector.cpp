#include "BooleanInspector.h"

#include <QDebug>

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
			if ( GetObjectProxy()->GetValue().ToBool() != clicked )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = clicked]()
				{
					qDebug() << "BooleanInspector.Redo" << value;
					proxy->SetValue( value );
					_CheckBox->setChecked( value );
				},
					[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue().ToBool()]()
				{
					qDebug() << "BooleanInspector.Undo" << value;
					proxy->SetValue( value );
					_CheckBox->setChecked( value );
				} );
			}
		} );
}
