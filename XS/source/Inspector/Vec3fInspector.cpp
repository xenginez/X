#include "Vec3fInspector.h"

#include <QHBoxLayout>

REG_WIDGET( XS::Vec3fInspector );

REG_INSPECTOR( XE::Vec3f, XS::Vec3fInspector );

XS::Vec3fInspector::Vec3fInspector( QWidget * parent /*= nullptr */ )
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
			_Z = new QDoubleSpinBox( this ); _Z->setPrefix( "Z " );
		}
		layout->addWidget( _X );
		layout->addWidget( _Y );
		layout->addWidget( _Z );
	}
	SetContentWidget( widget );
}

XS::Vec3fInspector::~Vec3fInspector()
{

}

void XS::Vec3fInspector::Refresh()
{
	disconnect( _X, nullptr );
	disconnect( _Y, nullptr );
	disconnect( _Z, nullptr );

	if ( auto attr = GetObjectProxy()->FindAttributeT< XE::RangeAttribute >() )
	{
		_X->setSingleStep( attr->GetStep() ); _X->setRange( attr->GetMin(), attr->GetMax() );
		_Y->setSingleStep( attr->GetStep() ); _Y->setRange( attr->GetMin(), attr->GetMax() );
		_Z->setSingleStep( attr->GetStep() ); _Z->setRange( attr->GetMin(), attr->GetMax() );
	}

	auto vec = GetObjectProxy()->GetValue().Value< XE::Vec3f >();
	_X->setValue( vec.x ); _Y->setValue( vec.y ); _Z->setValue( vec.z );

	connect( _X, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec3f>();
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
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec3f>();
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
	connect( _Z, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec3f>();
			if ( XE::Mathf::Abs( val - vec.z ) > std::numeric_limits<XE::float32>::epsilon() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), vec, value = val]() mutable
				{
					vec.z = value;

					proxy->SetValue( vec );
					_Z->setValue( value );
				},
					[this, proxy = GetObjectProxy(), vec]() mutable
				{
					proxy->SetValue( vec );
					_Z->setValue( vec.z );
				} );
			}
		} );
}
