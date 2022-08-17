#include "ContainerInspector.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QSpacerItem>
#include <QToolButton>
#include <QVBoxLayout>

#include "ui_PairInspector.h"

REG_WIDGET( XS::ContainerInspector );

namespace
{
	class ElementObjectProxy : public XS::ObjectProxy
	{
	public:
		ElementObjectProxy( XE::uint64 i, XE::VariantArray * arr, XS::ObjectProxy * parent )
			: XS::ObjectProxy( parent ), _Index( i ), _Array( arr )
		{

		}

	public:
		XE::MetaTypeCPtr GetType() const override
		{
			return _Array->at( _Index ).GetType();
		}

		const XE::String & GetName() const override
		{
			return _Array->at( _Index ).GetType()->GetFullName();
		}

		XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const override
		{
			return _Array->at( _Index ).GetType()->FindAttribute( type );
		}

	public:
		XE::Variant GetValue() const override
		{
			return _Array->at( _Index );
		}

		void SetValue( const XE::Variant & val ) override
		{
			_Array->at( _Index ) = val;
			GetParent()->SetValue( *_Array );
		}

	private:
		XE::uint64 _Index;
		XE::VariantArray * _Array;
	};
}

namespace Ui
{
	class ContainerInspector
	{
	public:
		QVBoxLayout * verticalLayout;
		QListWidget * list;
		QHBoxLayout * horizontalLayout;
		QSpacerItem * horizontalSpacer;
		QToolButton * add;
		QToolButton * sub;

		void setupUi( QWidget * Container )
		{
			if ( Container->objectName().isEmpty() )
				Container->setObjectName( QString::fromUtf8( "ContainerInspector" ) );
			Container->resize( 410, 508 );
			verticalLayout = new QVBoxLayout( Container );
			verticalLayout->setSpacing( 5 );
			verticalLayout->setObjectName( QString::fromUtf8( "verticalLayout" ) );
			verticalLayout->setContentsMargins( 0, 0, 0, 0 );
			list = new QListWidget( Container );
			list->setObjectName( QString::fromUtf8( "list" ) );

			verticalLayout->addWidget( list );

			horizontalLayout = new QHBoxLayout();
			horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
			horizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

			horizontalLayout->addItem( horizontalSpacer );

			add = new QToolButton( Container );
			add->setObjectName( QString::fromUtf8( "add" ) );
			add->setMinimumSize( QSize( 20, 20 ) );
			add->setMaximumSize( QSize( 20, 20 ) );

			horizontalLayout->addWidget( add );

			sub = new QToolButton( Container );
			sub->setObjectName( QString::fromUtf8( "sub" ) );
			sub->setMinimumSize( QSize( 20, 20 ) );
			sub->setMaximumSize( QSize( 20, 20 ) );

			horizontalLayout->addWidget( sub );


			verticalLayout->addLayout( horizontalLayout );

			QMetaObject::connectSlotsByName( Container );
		}
	};
}

XS::ContainerInspector::ContainerInspector( QWidget * parent /*= nullptr */ )
	: GroupInspector( parent ), ui( new Ui::ContainerInspector )
{
	ui->setupUi( this );

	connect( ui->add, &QToolButton::clicked, this, &XS::ContainerInspector::OnAddToolButtonClicked );
}

XS::ContainerInspector::~ContainerInspector()
{
	delete ui;
}

void XS::ContainerInspector::Refresh()
{
	ui->list->clear();

	_Array = GetObjectProxy()->GetValue().ToArray();

	for ( size_t i = 0; i < _Array.size(); i++ )
	{
		XS::ObjectProxy * proxy = new ElementObjectProxy( i, &_Array, GetObjectProxy() );

		auto inspector = XS::Inspector::Create( proxy, this );

		QListWidgetItem * item = new QListWidgetItem( ui->list );
		ui->list->setItemWidget( item, inspector );
	}
}

void XS::ContainerInspector::OnAddToolButtonClicked()
{
	if ( GetObjectProxy()->GetType()->GetType() == XE::MetaInfoType::CLASS )
	{
		auto cls = SP_CAST< const XE::MetaClass >( GetObjectProxy()->GetType() );
		// TODO: 
	}
	else
	{

	}

	auto val = GetObjectProxy()->GetValue();
	val.FromArray( _Array );
	GetObjectProxy()->SetValue( val );

	Refresh();
}

void XS::ContainerInspector::OnSubToolButtonClicked()
{
	_Array.pop_back();

	auto val = GetObjectProxy()->GetValue();
	val.FromArray( _Array );
	GetObjectProxy()->SetValue( val );

	Refresh();
}
