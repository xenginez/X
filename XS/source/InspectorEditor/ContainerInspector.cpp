#include "ContainerInspector.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QSpacerItem>
#include <QToolButton>
#include <QVBoxLayout>

REG_WIDGET( XS::ContainerInspector );

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
			Container->resize( 410, 500 );
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
			add->setIcon( QIcon( "SkinIcons:/images/inspector/icon_inspector_add.png" ) );

			horizontalLayout->addWidget( add );

			sub = new QToolButton( Container );
			sub->setObjectName( QString::fromUtf8( "sub" ) );
			sub->setMinimumSize( QSize( 20, 20 ) );
			sub->setMaximumSize( QSize( 20, 20 ) );
			sub->setIcon( QIcon( "SkinIcons:/images/inspector/icon_inspector_sub.png" ) );

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
	connect( ui->sub, &QToolButton::clicked, this, &XS::ContainerInspector::OnSubToolButtonClicked );
}

XS::ContainerInspector::~ContainerInspector()
{
	delete ui;
}

void XS::ContainerInspector::Refresh()
{
	ui->list->clear();

	_Container = GetObjectProxy()->GetValue();
	_Enumerator = SP_CAST< const XE::MetaClass >( GetObjectProxy()->GetType() )->GetEnumerator( _Container );

	for ( XE::uint64 i = 0; !_Enumerator->IsEnd(); _Enumerator->MoveNext(), ++i )
	{
		XS::ObjectProxy * proxy = new ElementObjectProxy( i, _Enumerator, GetObjectProxy() );

		auto inspector = XS::InspectorWidget::Create( proxy, this );

		QListWidgetItem * item = new QListWidgetItem( ui->list );
		item->setFlags( item->flags() | Qt::ItemIsSelectable );
		ui->list->setItemWidget( item, inspector );
	}
}

void XS::ContainerInspector::OnAddToolButtonClicked()
{
	auto old_value = _Container;

	PushUndoCommand( QString("Add %1 Enum Value").arg( GetObjectProxy()->GetName().c_str() ),
					 [this]()
					{
						XE::Variant element;

						auto type = _Enumerator->GetValueType();
						switch ( type->GetType() )
						{
						case XE::MetaInfoType::ENUM:
							element = SP_CAST< const XE::MetaEnum >( type )->GetDefaultValue();
							break;
						case XE::MetaInfoType::CLASS:
							element = SP_CAST< const XE::MetaClass >( type )->Construct( nullptr );
							break;
						}

						if ( ui->list->currentItem() != nullptr )
							_Enumerator->SetIndex( ui->list->currentIndex().row() );
						else
							_Enumerator->SetIndex( 0 );

						_Enumerator->InsertToCurrent( element );

						GetObjectProxy()->SetValue( _Container );

						Refresh();
					},
					 [this, old_value]()
					{
						GetObjectProxy()->SetValue( old_value );

						Refresh();
					} );
}

void XS::ContainerInspector::OnSubToolButtonClicked()
{
	auto old_value = _Container;

	PushUndoCommand( QString( "Add %1 Enum Value" ).arg( GetObjectProxy()->GetName().c_str() ),
					 [this]()
					{
						if ( ui->list->currentItem() != nullptr )
							_Enumerator->SetIndex( ui->list->currentIndex().row() );
						else
							_Enumerator->SetIndex( 0 );

						_Enumerator->EraseCurrent();

						GetObjectProxy()->SetValue( _Container );

						Refresh();
					},
					 [this, old_value]()
					{
						GetObjectProxy()->SetValue( old_value );

						Refresh();
					} );
}

void XS::ContainerInspector::OnRedo()
{
	GetObjectProxy()->SetValue( _Container );
}

void XS::ContainerInspector::OnUndo()
{
	GetObjectProxy()->SetValue( _Container );
}

void XS::ContainerInspector::OnSave()
{
	GetObjectProxy()->SetValue( _Container );
}
