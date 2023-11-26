#include "Vec4fInspector.h"

#include <QHBoxLayout>

REG_INSPECTOR( XE::Vec4f, XS::Vec4fInspector );

XS::Vec4fInspector::Vec4fInspector( QWidget * parent /*= nullptr */ )
	:InspectorWidget( parent )
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
			_W = new QDoubleSpinBox( this ); _W->setPrefix( "W " );
		}
		layout->addWidget( _X );
		layout->addWidget( _Y );
		layout->addWidget( _Z );
		layout->addWidget( _W );
	}
	SetContentWidget( widget );
}

XS::Vec4fInspector::~Vec4fInspector()
{

}

void XS::Vec4fInspector::Refresh()
{
	disconnect( _X, nullptr );
	disconnect( _Y, nullptr );
	disconnect( _Z, nullptr );
	disconnect( _W, nullptr );

	if ( auto attr = GetObjectProxy()->FindAttributeT< XE::RangeAttribute >() )
	{
		_X->setSingleStep( attr->GetStep() ); _X->setRange( attr->GetMin(), attr->GetMax() );
		_Y->setSingleStep( attr->GetStep() ); _Y->setRange( attr->GetMin(), attr->GetMax() );
		_Z->setSingleStep( attr->GetStep() ); _Z->setRange( attr->GetMin(), attr->GetMax() );
		_W->setSingleStep( attr->GetStep() ); _W->setRange( attr->GetMin(), attr->GetMax() );
	}

	auto vec = GetObjectProxy()->GetValue().Value< XE::Vec4f >();
	_X->setValue( vec.x ); _Y->setValue( vec.y ); _Z->setValue( vec.z ); _W->setValue( vec.w );

	connect( _X, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec4f>();
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
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec4f>();
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
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec4f>();
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
	connect( _W, QOverload< double >::of( &QDoubleSpinBox::valueChanged ), [this]( double val )
		{
			auto vec = GetObjectProxy()->GetValue().Value<XE::Vec4f>();
			if ( XE::Mathf::Abs( val - vec.w ) > std::numeric_limits<XE::float32>::epsilon() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), vec, value = val]() mutable
				{
					vec.w = value;

					proxy->SetValue( vec );
					_W->setValue( value );
				},
					[this, proxy = GetObjectProxy(), vec]() mutable
				{
					proxy->SetValue( vec );
					_W->setValue( vec.w );
				} );
			}
		} );
}
