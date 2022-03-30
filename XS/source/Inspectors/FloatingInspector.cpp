#include "FloatingInspector.h"

REG_WIDGET( XS::FloatingInspector );

REG_INSPECTOR( XE::float16, XS::FloatingInspector );
REG_INSPECTOR( XE::float32, XS::FloatingInspector );
REG_INSPECTOR( XE::float64, XS::FloatingInspector );

XS::FloatingInspector::FloatingInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	_QDoubleSpinBox = new QDoubleSpinBox( this );
	{
		_QDoubleSpinBox->setWrapping( true );
		_QDoubleSpinBox->setSingleStep( 1.0 );
	}
	SetContentWidget( _QDoubleSpinBox );
}

XS::FloatingInspector::~FloatingInspector()
{

}

void XS::FloatingInspector::Refresh()
{
	disconnect( _QDoubleSpinBox, nullptr );

	if ( auto attr = GetObjectProxy()->FindAttributeT< XE::RangeAttribute >() )
	{
		_QDoubleSpinBox->setMinimum( attr->GetMin() );
		_QDoubleSpinBox->setMaximum( attr->GetMax() );
	}
	else
	{
		auto type = GetObjectProxy()->GetType();
		if ( type == TypeID< XE::float16 >::Get() )
		{
			_QDoubleSpinBox->setMinimum( -32768.0f );
			_QDoubleSpinBox->setMaximum( +32767.0f );
		}
		else if ( type == TypeID< XE::float32 >::Get() )
		{
			_QDoubleSpinBox->setMinimum( -2147483648.0f );
			_QDoubleSpinBox->setMaximum( +2147483647.0f );
		}
		else if ( type == TypeID< XE::float64 >::Get() )
		{
			_QDoubleSpinBox->setMinimum( -9223372036854775808.0 );
			_QDoubleSpinBox->setMaximum( +9223372036854775807.0 );
		}
	}

	_QDoubleSpinBox->setValue( GetObjectProxy()->GetValue().ToFloat64() );

	connect( _QDoubleSpinBox, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			PushUndoCommand( GetObjectProxy()->GetName().c_str(),
				[this, proxy = GetObjectProxy(), value = val]()
			{
				proxy->SetValue( value );
			},
				[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue()]()
			{
				proxy->SetValue( value );
			} );
		} );
}
