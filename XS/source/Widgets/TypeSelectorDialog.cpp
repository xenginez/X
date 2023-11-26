#include "TypeSelectorDialog.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>

Q_DECLARE_METATYPE( XE::MetaInfoCPtr );

namespace Ui
{
	class TypeSelectorDialog
	{
	public:
		QVBoxLayout * verticalLayout;
		QHBoxLayout * horizontalLayout;
		QLineEdit * search;
		QToolButton * expand;
		QToolButton * collapse;
		QTreeWidget * tree;
		QHBoxLayout * horizontalLayout_2;
		QSpacerItem * horizontalSpacer;
		QPushButton * yes;
		QPushButton * cancel;

		void setupUi( QWidget * TypeSelectorDialog )
		{
			if ( TypeSelectorDialog->objectName().isEmpty() )
				TypeSelectorDialog->setObjectName( QString::fromUtf8( "TypeSelectorDialog" ) );
			TypeSelectorDialog->resize( 520, 510 );
			verticalLayout = new QVBoxLayout( TypeSelectorDialog );
			verticalLayout->setSpacing( 0 );
			verticalLayout->setObjectName( QString::fromUtf8( "verticalLayout" ) );
			verticalLayout->setContentsMargins( 0, 0, 0, 0 );
			horizontalLayout = new QHBoxLayout();
			horizontalLayout->setSpacing( 0 );
			horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
			search = new QLineEdit( TypeSelectorDialog );
			search->setObjectName( QString::fromUtf8( "search" ) );
			search->setClearButtonEnabled( true );

			horizontalLayout->addWidget( search );

			expand = new QToolButton( TypeSelectorDialog );
			expand->setObjectName( QString::fromUtf8( "expand" ) );
			expand->setMinimumSize( QSize( 20, 20 ) );
			expand->setMaximumSize( QSize( 20, 20 ) );

			horizontalLayout->addWidget( expand );

			collapse = new QToolButton( TypeSelectorDialog );
			collapse->setObjectName( QString::fromUtf8( "collapse" ) );
			collapse->setMinimumSize( QSize( 20, 20 ) );
			collapse->setMaximumSize( QSize( 20, 20 ) );

			horizontalLayout->addWidget( collapse );


			verticalLayout->addLayout( horizontalLayout );

			tree = new QTreeWidget( TypeSelectorDialog );
			QTreeWidgetItem * __qtreewidgetitem = new QTreeWidgetItem();
			__qtreewidgetitem->setText( 0, QString::fromUtf8( "1" ) );
			tree->setHeaderItem( __qtreewidgetitem );
			tree->setObjectName( QString::fromUtf8( "tree" ) );
			tree->setEditTriggers( QAbstractItemView::NoEditTriggers );
			tree->setHeaderHidden( true );

			verticalLayout->addWidget( tree );

			horizontalLayout_2 = new QHBoxLayout();
			horizontalLayout_2->setObjectName( QString::fromUtf8( "horizontalLayout_2" ) );
			horizontalLayout_2->setContentsMargins( 5, 5, 5, 5 );
			horizontalSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

			horizontalLayout_2->addItem( horizontalSpacer );

			yes = new QPushButton( TypeSelectorDialog );
			yes->setObjectName( QString::fromUtf8( "yes" ) );
			yes->setEnabled( false );

			horizontalLayout_2->addWidget( yes );

			cancel = new QPushButton( TypeSelectorDialog );
			cancel->setObjectName( QString::fromUtf8( "cancel" ) );

			horizontalLayout_2->addWidget( cancel );


			verticalLayout->addLayout( horizontalLayout_2 );
		}
	};
}

XS::TypeSelectorDialog::TypeSelectorDialog( Type type /*= Type::ALL*/, QWidget * parent /*= nullptr */ )
	:QDialog( parent ), ui( new Ui::TypeSelectorDialog ), _Type( type )
{
	ui->setupUi( this );

	ui->expand->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_expand.png" ) );
	ui->collapse->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_collapse.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	FillTree();

	connect( ui->expand, &QToolButton::clicked, [this]() { ui->tree->expandAll(); } );
	connect( ui->collapse, &QToolButton::clicked, [this]() { ui->tree->collapseAll(); } );
	connect( ui->search, &QLineEdit::textChanged, this, &XS::TypeSelectorDialog::OnSearchTextChanged );
	connect( ui->yes, &QPushButton::clicked, this, &XS::TypeSelectorDialog::OnYesPushButtonClicked );
	connect( ui->cancel, &QPushButton::clicked, this, &XS::TypeSelectorDialog::OnCancelPushButtonClicked );
	connect( ui->tree, &QTreeWidget::itemClicked, this, &XS::TypeSelectorDialog::OnTreeViewItemClicked );
	connect( ui->tree, &QTreeWidget::itemDoubleClicked, this, &XS::TypeSelectorDialog::OnTreeViewItemDoubleClicked );
}

XS::TypeSelectorDialog::~TypeSelectorDialog()
{
	delete ui;
}

XE::MetaID XS::TypeSelectorDialog::GetSelectTypeID() const
{
	return XE::MetaID( _MetaType->GetHashCode() );
}

const XE::MetaTypeCPtr & XS::TypeSelectorDialog::GetSelectType() const
{
	return _MetaType;
}

void XS::TypeSelectorDialog::OnYesPushButtonClicked()
{
	_MetaType = SP_CAST< const XE::MetaType >( ui->tree->currentItem()->data( 0, Qt::UserRole + 1 ).value<XE::MetaInfoCPtr>() );

	accept();
}

void XS::TypeSelectorDialog::OnCancelPushButtonClicked()
{
	reject();
}

void XS::TypeSelectorDialog::OnSearchTextChanged( const QString & text )
{
	if ( text.isEmpty() )
	{
		for ( QTreeWidgetItemIterator it( ui->tree ); *it; ++it )
		{
			( *it )->setHidden( false );
		}
	}
	else
	{
		for ( QTreeWidgetItemIterator it( ui->tree ); *it; ++it )
		{
			auto item = *it;
			if ( item->text( 0 ).contains( text, Qt::CaseInsensitive ) == false )
			{
				item->setHidden( true );
			}
			else
			{
				item->setHidden( false );

				for ( item = item->parent(); item != nullptr; item = item->parent() )
				{
					item->setHidden( false );

					ui->tree->expandItem( item );
				}
			}
		}
	}
}

void XS::TypeSelectorDialog::OnTreeViewItemClicked( QTreeWidgetItem * item, int column )
{
	if ( auto info = item->data( 0, Qt::UserRole + 1 ).value<XE::MetaInfoCPtr>() )
	{
		if ( info->GetType() == XE::MetaInfoType::ENUM || info->GetType() == XE::MetaInfoType::CLASS )
		{
			ui->yes->setEnabled( true );
		}
	}
	else
	{
		ui->yes->setEnabled( false );
	}
}

void XS::TypeSelectorDialog::OnTreeViewItemDoubleClicked( QTreeWidgetItem * item, int column )
{
	if ( auto info = item->data( 0, Qt::UserRole + 1 ).value<XE::MetaInfoCPtr>() )
	{
		if ( info->GetType() == XE::MetaInfoType::ENUM || info->GetType() == XE::MetaInfoType::CLASS )
		{
			OnYesPushButtonClicked();
		}
	}
}

void XS::TypeSelectorDialog::FillTree()
{
	QList< XE::MetaEnumCPtr > enums;
	QList< XE::MetaClassCPtr > classes;

	if ( ( _Type & Type::ENUM ) != 0 )
	{
		XE::Reflection::VisitEnum( [&]( const XE::MetaEnumCPtr & val )
		{
			enums.push_back( val );
		} );
	}
	if ( ( _Type & Type::CLASS ) != 0 )
	{
		XE::Reflection::VisitClass( [&]( const XE::MetaClassCPtr & val )
		{
			classes.push_back( val );
		} );
	}

	std::sort( enums.begin(), enums.end(), []( const auto & left, const auto & right )
	{
		return left->GetFullName() < right->GetFullName();
	} );
	std::sort( classes.begin(), classes.end(), []( const auto & left, const auto & right )
	{
		if ( left->IsAbstract() != right->IsAbstract() )
		{
			return left->IsAbstract();
		}

		return left->GetFullName() < right->GetFullName();
	} );

	auto enum_item = new QTreeWidgetItem( ui->tree );
	enum_item->setText( 0, tr( "Enums" ) );
	enum_item->setSizeHint( 0, QSize( ui->tree->width(), 30 ) );
	auto class_item = new QTreeWidgetItem( ui->tree );
	class_item->setText( 0, tr( "Classes" ) );
	class_item->setSizeHint( 0, QSize( ui->tree->width(), 30 ) );

	QMap<XE::MetaClassCPtr, QTreeWidgetItem *> item_map;

	for ( const auto & it : enums )
	{
		auto item = new QTreeWidgetItem( enum_item );
		item->setText( 0, QString::fromUtf8( it->GetFullName().c_str() ) );
		item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( SP_CAST< const XE::MetaInfo>( it ) ) );
	}

	for ( const auto & it : classes )
	{
		auto parent = class_item;
		if ( it->GetSuper() != nullptr )
		{
			auto iter = item_map.find( it->GetSuper() );
			if ( iter != item_map.end() )
			{
				parent = iter.value();
			}
		}

		auto item = new QTreeWidgetItem( parent );
		item->setText( 0, QString::fromUtf8( it->GetFullName().c_str() ) );
		item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( SP_CAST< const XE::MetaInfo>( it ) ) );
		item_map.insert( it, item );
	}
}
