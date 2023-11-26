#include "IntegerInspector.h"

REG_INSPECTOR( XE::int8, XS::IntegerInspector );
REG_INSPECTOR( XE::int16, XS::IntegerInspector );
REG_INSPECTOR( XE::int32, XS::IntegerInspector );
REG_INSPECTOR( XE::int64, XS::IntegerInspector );
REG_INSPECTOR( XE::uint8, XS::IntegerInspector );
REG_INSPECTOR( XE::uint16, XS::IntegerInspector );
REG_INSPECTOR( XE::uint32, XS::IntegerInspector );
REG_INSPECTOR( XE::uint64, XS::IntegerInspector );

XS::IntegerInspector::IntegerInspector( QWidget * parent /*= nullptr */ )
	:XS::InspectorWidget( parent )
{
	_QSpinBox = new QSpinBox( this );
	{
		_QSpinBox->setWrapping( true );
		_QSpinBox->setSingleStep( 1 );
	}
	SetContentWidget( _QSpinBox );
}

XS::IntegerInspector::~IntegerInspector()
{

}

void XS::IntegerInspector::Refresh()
{
	disconnect( _QSpinBox, nullptr );

	if ( auto attr = GetObjectProxy()->FindAttributeT< XE::RangeAttribute >() )
	{
		_QSpinBox->setMinimum( attr->GetMin() );
		_QSpinBox->setMaximum( attr->GetMax() );
		_QSpinBox->setSingleStep( attr->GetStep() );
	}
	else
	{
		auto type = GetObjectProxy()->GetType();
		if ( type == TypeID< XE::int8 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::int8 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::int8 >::max() );
		}
		else if ( type == TypeID< XE::int16 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::int16 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::int16 >::max() );
		}
		else if ( type == TypeID< XE::int32 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::int32 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::int32 >::max() );
		}
		else if ( type == TypeID< XE::int64 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::int64 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::int64 >::max() );
		}
		else if ( type == TypeID< XE::uint8 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::uint8 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::uint8 >::max() );
		}
		else if ( type == TypeID< XE::uint16 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::uint16 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::uint16 >::max() );
		}
		else if ( type == TypeID< XE::uint32 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::uint32 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::uint32 >::max() );
		}
		else if ( type == TypeID< XE::uint64 >::Get() )
		{
			_QSpinBox->setMinimum( std::numeric_limits< XE::uint64 >::min() );
			_QSpinBox->setMaximum( std::numeric_limits< XE::uint64 >::max() );
		}
	}

	_QSpinBox->setValue( GetObjectProxy()->GetValue().ToInt32() );

	connect( _QSpinBox, QOverload< int >::of( &QSpinBox::valueChanged ), [this]( int val )
		{
			if ( val != GetObjectProxy()->GetValue().ToInt32() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = val]()
				{
					proxy->SetValue( value );
					_QSpinBox->setValue( value );
				},
					[this, proxy = GetObjectProxy(), value = GetObjectProxy()->GetValue().ToInt32()]()
				{
					proxy->SetValue( value );
					_QSpinBox->setValue( value );
				} );
			}
		} );
}
