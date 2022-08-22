#include "Vec2fInspector.h"

#include <QHBoxLayout>

REG_INSPECTOR( XE::Vec2f, XS::Vec2fInspector );

XS::Vec2fInspector::Vec2fInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	QWidget * widget = new QWidget( this );
	{
		QHBoxLayout * layout = new QHBoxLayout( this );
		{
			layout->setSpacing( 5 );
			layout->setObjectName( QString::fromUtf8( "layout" ) );
			layout->setContentsMargins( 0, 0, 0, 0 );

			_X = new QDoubleSpinBox( this ); _X->setPrefix( "X " );
			_Y = new QDoubleSpinBox( this ); _Y->setPrefix( "Y " );
		}
		layout->addWidget( _X ); layout->addWidget( _Y );
	}
	SetContentWidget( widget );
}

XS::Vec2fInspector::~Vec2fInspector()
{

}

void XS::Vec2fInspector::Refresh()
{
	disconnect( _X, nullptr );
	disconnect( _Y, nullptr );

	if ( auto attr = GetObjectProxy()->FindAttributeT< XE::RangeAttribute >() )
	{
		_X->setSingleStep( attr->GetStep() ); _X->setRange( attr->GetMin(), attr->GetMax() );
		_Y->setSingleStep( attr->GetStep() ); _Y->setRange( attr->GetMin(), attr->GetMax() );
	}

	auto vec = GetObjectProxy()->GetValue().Value< XE::Vec2f >();
	_X->setValue( vec.x ); _Y->setValue( vec.y );

	connect( _X, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec2f>();
			if ( XE::Mathf::Abs( val - vec.x ) > std::numeric_limits<XE::float32>::epsilon() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), vec, value = val]() mutable
				{
					vec.x = value;

					proxy->SetValue( vec );
					_X->setValue( value );
				},
					[this, proxy = GetObjectProxy(), vec]() mutable
				{
					proxy->SetValue( vec );
					_X->setValue( vec.x );
				} );
			}
		} );
	connect( _Y, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec2f>();
			if ( XE::Mathf::Abs( val - vec.y ) > std::numeric_limits<XE::float32>::epsilon() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), vec, value = val]() mutable
				{
					vec.y = value;

					proxy->SetValue( vec );
					_Y->setValue( value );
				},
					[this, proxy = GetObjectProxy(), vec]() mutable
				{
					proxy->SetValue( vec );
					_Y->setValue( vec.y );
				} );
			}
		} );
}
