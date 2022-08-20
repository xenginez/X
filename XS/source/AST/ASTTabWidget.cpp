#include "ASTTabWidget.h"

#include "ui_ASTTabWidget.h"

#include <fstream>

#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include <QItemDelegate>

Q_DECLARE_METATYPE( XE::ASTInfoMetaPtr );

namespace
{
	enum ASTType
	{
		NONE = 0,
		ENUM = 1 << 1,
		CLASS = 1 << 2,
		MODULE = 1 << 3,
		METHOD = 1 << 4,
		PROPERTY = 1 << 5,
		FUNCTION = 1 << 6,
		VARIABLE = 1 << 7,
		GROUP = 1 << 8,
		VALUE = 1 << 9,
		LOCAL = 1 << 10,
		PARAMETER = 1 << 11,
		ENUM_VALUE = ENUM | VALUE,
		METHOD_PARAMETER = METHOD | PARAMETER,
		METHOD_LOCAL_VARIABLE = METHOD | LOCAL,
		FUNCTION_PARAMETER = FUNCTION | PARAMETER,
		FUNCTION_LOCAL_VARIABLE = FUNCTION | LOCAL,
	};

	static constexpr int AST_PTR_ROLE = Qt::UserRole + 1;
	static constexpr int AST_TYPE_ROLE = Qt::UserRole + 2;
	static constexpr const char * UUID_NAME = "AST_UUID";
	static constexpr const char * PATH_NAME = "AST_PATH";
	static constexpr const char * TREE_ICON = "SkinIcons:/images/icons/icon_asset_ast.svg";
	static constexpr const char * ENUM_ICON = "SkinIcons:/images/ast/icon_ast_enum.png";
	static constexpr const char * CLASS_ICON = "SkinIcons:/images/ast/icon_ast_class.png";
	static constexpr const char * MODULE_ICON = "SkinIcons:/images/ast/icon_ast_module.png";
	static constexpr const char * METHOD_ICON = "SkinIcons:/images/ast/icon_ast_method.png";
	static constexpr const char * PROPERTY_ICON = "SkinIcons:/images/ast/icon_ast_property.png";
	static constexpr const char * FUNCTION_ICON = "SkinIcons:/images/ast/icon_ast_function.png";
	static constexpr const char * VARIABLE_ICON = "SkinIcons:/images/ast/icon_ast_variable.png";
	static constexpr const char * PARAMETER_ICON = "SkinIcons:/images/ast/icon_ast_argument.png";
	static constexpr const char * LOCAL_VARIABLE_ICON = "SkinIcons:/images/ast/icon_ast_local.png";

	class TreeItemDelegate : public QItemDelegate
	{
	public:
		TreeItemDelegate( QObject * parent = nullptr )
			:QItemDelegate( parent )
		{

		}

		void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override
		{
			if ( ( index.model()->flags( index ) & Qt::ItemIsEditable ) == 0 )
			{
				auto r = option.rect;
				r.setTop( r.top() + 1 );
				r.setBottom( r.bottom() - 1 );
				painter->fillRect( r, qobject_cast<QWidget *>( parent() )->palette().brush( QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window ) );
			}

			QItemDelegate::paint( painter, option, index );
		}

		QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const override
		{
			QSize size = QItemDelegate::sizeHint( option, index );

			if ( ( index.model()->flags( index ) & Qt::ItemFlag::ItemIsEditable ) == 0 )
			{
				size.setHeight( size.height() * 2 );
			}
			else
			{
				size.setHeight( size.height() * 1.5f );
			}

			return size;
		}
	};
}

XS::ASTTabWidget::ASTTabWidget( QWidget * parent /*= nullptr */ )
	: Widget( parent ), ui( new Ui::ASTTabWidget )
{
	ui->setupUi( this );

	ui->expand->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_expand.png" ) );
	ui->collapse->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_collapse.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setSizes( { 3000, 7000 } );

	ui->tree->setFrameStyle( QFrame::NoFrame );
	ui->tree->setItemDelegate( new TreeItemDelegate( this ) );
	ui->tree->setStyleSheet( "QTreeWidget{ outline:0px; };" );

	connect( ui->expand, &QToolButton::clicked, [this]() { ui->tree->expandAll(); } );
	connect( ui->collapse, &QToolButton::clicked, [this]() { ui->tree->collapseAll(); } );
	connect( ui->search, &QLineEdit::textChanged, this, &XS::ASTTabWidget::OnSearchTextChanged );
	connect( ui->tree, &QTreeWidget::itemChanged, this, &XS::ASTTabWidget::OnTreeWidgetItemChanged );
	connect( ui->tree, &QTreeWidget::itemDoubleClicked, this, &XS::ASTTabWidget::OnTreeWidgetItemDoubleClicked );
	connect( ui->tree, &QTreeWidget::customContextMenuRequested, this, &XS::ASTTabWidget::OnTreeWidgetCustomContextMenuRequested );
}

XS::ASTTabWidget::~ASTTabWidget()
{

}

void XS::ASTTabWidget::OnSearchTextChanged( const QString & text )
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

void XS::ASTTabWidget::OnTreeWidgetItemChanged( QTreeWidgetItem * item, int col )
{
	XE::ASTInfoMetaPtr ast = item->data( 0, AST_PTR_ROLE ).value<XE::ASTInfoMetaPtr>();
	int type = item->data( 0, AST_TYPE_ROLE ).toInt();

	if (ast)
	{
		if ( ast->Name != item->text( 0 ).toStdString() )
		{
			QRegExp exp( "^[_a-zA-Z][_a-zA-Z0-9]+$" );

			if ( exp.exactMatch( item->text( 0 ) ) )
			{
				PushUndoCommand( QString( R"(Enum Name '%1' -> '%2')" ).arg( ast->Name.c_str() ).arg( item->text( 0 ) ),
								 [ast, item, text = item->text( 0 )]() { ast->Name = text.toStdString(); },
								 [ast, item, text = ast->Name]() { ast->Name = text; item->setText( 0, QString::fromUtf8( text.c_str() ) ); } );
			}
			else
			{
				item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
			}
		}
		else if ( type == ASTType::ENUM )
		{
			XE::ASTInfoEnumPtr e = SP_CAST< XE::ASTInfoEnum >( ast );
			bool old_flag = e->Flag;
			bool new_flag = item->checkState( 0 ) == Qt::Checked;

			if ( old_flag != new_flag )
			{
				PushUndoCommand( QString( "%1 Flag" ).arg( item->text( 0 ) ),
								 [e, item, new_flag]()
				{
					e->Flag = new_flag;
					item->setCheckState( 0, new_flag ? Qt::CheckState::Checked : Qt::CheckState::Unchecked );
					item->setToolTip( 0, item->text( 0 ) + ( e->Flag ? tr( " Flag" ) : QString() ) );
				},
								 [e, item, old_flag]()
				{
					e->Flag = old_flag;
					item->setCheckState( 0, old_flag ? Qt::CheckState::Checked : Qt::CheckState::Unchecked );
					item->setToolTip( 0, item->text( 0 ) + ( e->Flag ? tr( " Flag" ) : QString() ) );
				} );
			}
		}
	}
}

void XS::ASTTabWidget::OnTreeWidgetItemDoubleClicked( QTreeWidgetItem * item, int col )
{
	if ( ( item->flags() & Qt::ItemFlag::ItemIsEditable ) != 0 )
	{

	}
}

void XS::ASTTabWidget::OnTreeWidgetCustomContextMenuRequested( const QPoint & pos )
{
#define CAST_TYPE( TYPE ) ( ( type & ASTType::##TYPE ) != 0 )
#define CASE_BEG if ( false ){}
#define CASE_END else { return; }
#define CASE( TYPE ) else if( CAST_TYPE( TYPE ) )

	auto item = ui->tree->itemAt( pos );
	if ( item != nullptr )
	{
		int type = item->data( 0, AST_TYPE_ROLE ).toInt();

		if ( CAST_TYPE( GROUP ) )
		{
			QMenu menu( this );
			{
				CASE_BEG
				CASE( ENUM )
				{
					menu.addAction( tr( "Create Enum" ), [this, item]() { CreateEnum( item ); } );
				}
				CASE( CLASS )
				{
					menu.addAction( tr( "Create Class" ), [this, item]() { CreateClass( item ); } );
				}
				CASE( MODULE )
				{
					menu.addAction( tr( "Create Module" ), [this, item]() { CreateModule( item ); } );
				}
				CASE( METHOD )
				{
					menu.addAction( tr( "Create Method" ), [this, item]() { CreateMethod( item ); } );
				}
				CASE( PROPERTY )
				{
					menu.addAction( tr( "Create Property" ), [this, item]() { CreateProperty( item ); } );
				}
				CASE( FUNCTION )
				{
					menu.addAction( tr( "Create Function" ), [this, item]() { CreateFunction( item ); } );
				}
				CASE( VARIABLE )
				{
					menu.addAction( tr( "Create Variable" ), [this, item]() { CreateVariable( item ); } );
				}
				CASE( METHOD_PARAMETER )
				{
					menu.addAction( tr( "Create Method Parameter" ), [this, item]() { CreateMethodParameter( item ); } );
				}
				CASE( FUNCTION_PARAMETER )
				{
					menu.addAction( tr( "Create Function Parameter" ), [this, item]() { CreateFunctionParameter( item ); } );
				}
				CASE( METHOD_LOCAL_VARIABLE )
				{
					menu.addAction( tr( "Create Method Local Variable" ), [this, item]() { CreateMethodLocalVariable( item ); } );
				}
				CASE( FUNCTION_LOCAL_VARIABLE )
				{
					menu.addAction( tr( "Create Function Local Variable" ), [this, item]() { CreateFunctionLocalVariable( item ); } );
				}
				CASE_END
			}
			menu.exec( QCursor::pos() );
		}
		else
		{
			QMenu menu( this );
			{
				CASE_BEG
				CASE( ENUM )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( CLASS )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( MODULE )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( METHOD )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( PROPERTY )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( FUNCTION )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( VARIABLE )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( METHOD_PARAMETER )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( FUNCTION_PARAMETER )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( METHOD_LOCAL_VARIABLE )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE( FUNCTION_LOCAL_VARIABLE )
				{
					menu.addAction( tr( "Rename" ), [&]() {} );
					menu.addAction( tr( "Remove" ), [&]() {} );
				}
				CASE_END
			}
			menu.exec( QCursor::pos() );
		}
	}

#undef CASE
#undef CASE_END
#undef CASE_BEG
#undef CAST_TYPE
}

void XS::ASTTabWidget::showEvent( QShowEvent * event )
{
	XS::Widget::showEvent( event );

	auto path = property( PATH_NAME ).value<QFileInfo>();

	std::ifstream ifs( path.absoluteFilePath().toStdString() );
	if ( ifs.is_open() )
	{
		XE::JsonIArchive archive( ifs );

		archive >> _Module;

		if ( _Module )
		{
			FillTree();
		}
	}
}

void XS::ASTTabWidget::OnRedo()
{
	auto tab = GetParent< QTabWidget >();
	if ( tab != nullptr )
	{
		auto i = tab->indexOf( this );

		QString text = tab->tabText( i );
		if ( !text.startsWith( "* " ) )
		{
			tab->setTabText( i, "* " + text );
		}
	}
}

void XS::ASTTabWidget::OnUndo()
{
	auto tab = GetParent< QTabWidget >();
	if ( tab != nullptr )
	{
		auto i = tab->indexOf( this );

		QString text = tab->tabText( i );
		if ( text.startsWith( "* " ) )
		{
			tab->setTabText( i, text.right( text.size() - 2 ) );
		}
	}
}

void XS::ASTTabWidget::OnSave()
{
	auto tab = GetParent< QTabWidget >();
	if ( tab != nullptr )
	{
		auto i = tab->indexOf( this );

		QString text = tab->tabText( i );
		if ( text.startsWith( "* " ) )
		{
			tab->setTabText( i, text.right( text.size() - 2 ) );
		}
	}

	auto path = property( PATH_NAME ).value<QFileInfo>();

	std::stringstream json;
	XE::JsonOArchive archive( json );
	archive << _Module;

	QFile file( path.absoluteFilePath() );
	if ( file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		std::string data = json.str();
		file.write( data.c_str(), data.size() );
	}
	else
	{
		XE_ERROR( "{%1} file open failure", XE::String( path.absoluteFilePath().toStdString() ) );
	}
}

void XS::ASTTabWidget::FillTree()
{
	ui->tree->clear();

	QTreeWidgetItem * enum_item = new QTreeWidgetItem( ui->tree );
	{
		enum_item->setText( 0, tr( "Enum" ) );
		enum_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		enum_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::ENUM | ASTType::GROUP ) );
		auto font = enum_item->font( 0 ); font.setBold( true ); enum_item->setFont( 0, font );

		for ( const auto & it : _Module->Enums )
		{
			FillEnum( it, enum_item );
		}
	}
	QTreeWidgetItem * class_item = new QTreeWidgetItem( ui->tree );
	{
		class_item->setText( 0, tr( "Class" ) );
		class_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		class_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::CLASS | ASTType::GROUP ) );
		auto font = class_item->font( 0 ); font.setBold( true ); class_item->setFont( 0, font );

		for ( const auto & it : _Module->Classes )
		{
			FillClass( it, class_item );
		}
	}
	QTreeWidgetItem * module_item = new QTreeWidgetItem( ui->tree );
	{
		module_item->setText( 0, tr( "Module" ) );
		module_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		module_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::MODULE | ASTType::GROUP ) );
		auto font = module_item->font( 0 ); font.setBold( true ); module_item->setFont( 0, font );

		for ( const auto & it : _Module->Modules )
		{
			FillModule( it, module_item );
		}
	}
	QTreeWidgetItem * function_item = new QTreeWidgetItem( ui->tree );
	{
		function_item->setText( 0, tr( "Function" ) );
		function_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		function_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::GROUP ) );
		auto font = function_item->font( 0 ); font.setBold( true ); function_item->setFont( 0, font );

		for ( const auto & it : _Module->Functions )
		{
			FillFunction( it, function_item );
		}
	}
	QTreeWidgetItem * variable_item = new QTreeWidgetItem( ui->tree );
	{
		variable_item->setText( 0, tr( "Variable" ) );
		variable_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		variable_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::VARIABLE | ASTType::GROUP ) );
		auto font = variable_item->font( 0 ); font.setBold( true ); variable_item->setFont( 0, font );

		for ( const auto & it : _Module->Variables )
		{
			FillVariable( it, variable_item );
		}
	}

}

QTreeWidgetItem * XS::ASTTabWidget::FillEnum( const XE::ASTInfoEnumPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( ENUM_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsUserCheckable | Qt::ItemFlag::ItemIsEditable );
		item->setCheckState( 0, ast->Flag ? Qt::CheckState::Checked : Qt::CheckState::Unchecked );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::ENUM );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
		item->setToolTip( 0, item->text( 0 ) + ( ast->Flag ? tr( " Flag" ) : QString() ) );
	}

	for ( const auto & it : ast->Elements )
	{
		QTreeWidgetItem * element_item = new QTreeWidgetItem( item );
		{
			element_item->setText( 0, QString::fromUtf8( it.c_str() ) );
			element_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
			element_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::ENUM_VALUE ) );
		}
	}

	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillClass( const XE::ASTInfoClassPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( CLASS_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::CLASS );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}

	QTreeWidgetItem * method_item = new QTreeWidgetItem( item );
	{
		method_item->setText( 0, tr( "Method" ) );
		method_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		method_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::METHOD | ASTType::GROUP ) );
		auto font = method_item->font( 0 ); font.setBold( true ); method_item->setFont( 0, font );

		for ( const auto & it : ast->Methods )
		{
			FillMethod( it, method_item );
		}
	}

	QTreeWidgetItem * property_item = new QTreeWidgetItem( item );
	{
		property_item->setText( 0, tr( "Property" ) );
		property_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		property_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::PROPERTY | ASTType::GROUP ) );
		auto font = property_item->font( 0 ); font.setBold( true ); property_item->setFont( 0, font );

		for ( const auto & it : ast->Propertys )
		{
			FillProperty( it, property_item );
		}
	}

	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillModule( const XE::ASTInfoModulePtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( MODULE_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::MODULE );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}

	QTreeWidgetItem * enum_item = new QTreeWidgetItem( item );
	{
		enum_item->setText( 0, tr( "Enum" ) );
		enum_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		enum_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::ENUM | ASTType::GROUP ) );
		auto font = enum_item->font( 0 ); font.setBold( true ); enum_item->setFont( 0, font );

		for ( const auto & it : ast->Enums )
		{
			FillEnum( it, enum_item );
		}
	}
	QTreeWidgetItem * class_item = new QTreeWidgetItem( item );
	{
		class_item->setText( 0, tr( "Class" ) );
		class_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		class_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::CLASS | ASTType::GROUP ) );
		auto font = class_item->font( 0 ); font.setBold( true ); class_item->setFont( 0, font );

		for ( const auto & it : ast->Classes )
		{
			FillClass( it, class_item );
		}
	}
	QTreeWidgetItem * module_item = new QTreeWidgetItem( item );
	{
		module_item->setText( 0, tr( "Module" ) );
		module_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		module_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::MODULE | ASTType::GROUP ) );
		auto font = module_item->font( 0 ); font.setBold( true ); module_item->setFont( 0, font );

		for ( const auto & it : ast->Modules )
		{
			FillModule( it, module_item );
		}
	}
	QTreeWidgetItem * function_item = new QTreeWidgetItem( item );
	{
		function_item->setText( 0, tr( "Function" ) );
		function_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		function_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::GROUP ) );
		auto font = function_item->font( 0 ); font.setBold( true ); function_item->setFont( 0, font );

		for ( const auto & it : ast->Functions )
		{
			FillFunction( it, function_item );
		}
	}
	QTreeWidgetItem * variable_item = new QTreeWidgetItem( item );
	{
		variable_item->setText( 0, tr( "Variable" ) );
		variable_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		variable_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::VARIABLE | ASTType::GROUP ) );
		auto font = variable_item->font( 0 ); font.setBold( true ); variable_item->setFont( 0, font );

		for ( const auto & it : ast->Variables )
		{
			FillVariable( it, variable_item );
		}
	}

	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillMethod( const XE::ASTInfoMethodPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( METHOD_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::METHOD );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}

	QTreeWidgetItem * parameter_item = new QTreeWidgetItem( item );
	{
		parameter_item->setText( 0, tr( "Parameter" ) );
		parameter_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		parameter_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::METHOD | ASTType::PARAMETER | ASTType::GROUP ) );
		auto font = parameter_item->font( 0 ); font.setBold( true ); parameter_item->setFont( 0, font );

		for ( const auto & it : ast->Parameters )
		{
			QTreeWidgetItem * name_item = new QTreeWidgetItem( parameter_item );
			{
				name_item->setIcon( 0, QIcon( PARAMETER_ICON ) );
				name_item->setText( 0, QString::fromUtf8( it.first.c_str() ) );
				name_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
				name_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::METHOD | ASTType::PARAMETER ) );
			}

//			ui->tree->setItemWidget( name_item, 0, XS::Inspector::Create( new VariantObjectProxy( it.second ), this ) );
		}
	}

	QTreeWidgetItem * local_item = new QTreeWidgetItem( item );
	{
		local_item->setText( 0, tr( "LocalVariable" ) );
		local_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		local_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::METHOD | ASTType::LOCAL | ASTType::GROUP ) );
		auto font = local_item->font( 0 ); font.setBold( true ); local_item->setFont( 0, font );

		for ( const auto & it : ast->LocalVariables )
		{
			QTreeWidgetItem * name_item = new QTreeWidgetItem( local_item );
			{
				name_item->setIcon( 0, QIcon( LOCAL_VARIABLE_ICON ) );
				name_item->setText( 0, QString::fromUtf8( it.first.c_str() ) );
				name_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
				name_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::METHOD | ASTType::LOCAL ) );
			}

//			ui->tree->setItemWidget( name_item, 0, XS::Inspector::Create( new VariantObjectProxy( it.second ), this ) );
		}
	}

	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillProperty( const XE::ASTInfoPropertyPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( PROPERTY_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::PROPERTY );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}
	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillFunction( const XE::ASTInfoFunctionPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( FUNCTION_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::FUNCTION );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}

	QTreeWidgetItem * parameter_item = new QTreeWidgetItem( item );
	{
		parameter_item->setText( 0, tr( "Parameter" ) );
		parameter_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		parameter_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::PARAMETER | ASTType::GROUP ) );
		auto font = parameter_item->font( 0 ); font.setBold( true ); parameter_item->setFont( 0, font );

		for ( const auto & it : ast->Parameters )
		{
			QTreeWidgetItem * name_item = new QTreeWidgetItem( parameter_item );
			{
				name_item->setIcon( 0, QIcon( PARAMETER_ICON ) );
				name_item->setText( 0, QString::fromUtf8( it.first.c_str() ) );
				name_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
				name_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::PARAMETER ) );
			}

//			ui->tree->setItemWidget( name_item, 1, XS::Inspector::Create( new VariantObjectProxy( it.second ), this ) );
		}
	}

	QTreeWidgetItem * local_item = new QTreeWidgetItem( item );
	{
		local_item->setText( 0, tr( "LocalVariable" ) );
		local_item->setFlags( Qt::ItemFlag::ItemIsEnabled );
		local_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::LOCAL | ASTType::GROUP ) );
		auto font = local_item->font( 0 ); font.setBold( true ); local_item->setFont( 0, font );

		for ( const auto & it : ast->LocalVariables )
		{
			QTreeWidgetItem * name_item = new QTreeWidgetItem( local_item );
			{
				name_item->setIcon( 0, QIcon( LOCAL_VARIABLE_ICON ) );
				name_item->setText( 0, QString::fromUtf8( it.first.c_str() ) );
				name_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
				name_item->setData( 0, AST_TYPE_ROLE, (int)( ASTType::FUNCTION | ASTType::LOCAL ) );
			}

//			ui->tree->setItemWidget( name_item, 1, XS::Inspector::Create( new VariantObjectProxy( it.second ), this ) );
		}
	}

	return item;
}

QTreeWidgetItem * XS::ASTTabWidget::FillVariable( const XE::ASTInfoVariablePtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setIcon( 0, QIcon( VARIABLE_ICON ) );
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsEditable );
		item->setData( 0, AST_TYPE_ROLE, (int)ASTType::VARIABLE );
		item->setData( 0, AST_PTR_ROLE, QVariant::fromValue( SP_CAST< XE::ASTInfoMeta >( ast ) ) );
	}
	return item;
}

void XS::ASTTabWidget::CreateEnum( QTreeWidgetItem * group )
{
	auto module_ptr = SP_CAST< XE::ASTInfoModule >( GetASTParent( group, ASTType::MODULE ) );

	QString name = "NewEnum";
	for ( int i = 1; std::find_if( module_ptr->Enums.begin(), module_ptr->Enums.end(), [&]( const auto & val ) { return val->Name == name.toStdString(); } ) != module_ptr->Enums.end(); i++ )
	{
		name = QString( "NewEnum_%1" ).arg( i );
	}

	auto ast = XE::MakeShared< XE::ASTInfoEnum >();
	ast->Name = name.toStdString();

	PushUndoCommand( tr( "Create Enum " ) + name,
					 [this, ast, group, module_ptr]()
					{
						module_ptr->Enums.push_back( ast );

						ui->tree->setCurrentItem( FillEnum( ast, group ) );
					},
					 [this, ast, group, module_ptr]()
					{
						module_ptr->Enums.erase( std::find( module_ptr->Enums.begin(), module_ptr->Enums.end(), ast ) );

						for ( int i = 0; i < group->childCount(); i++ )
						{
							if ( group->child( i )->data( 0, AST_PTR_ROLE ).value<XE::ASTInfoMetaPtr>() == ast )
							{
								delete group->takeChild( i );
								break;
							}
						}
					} );
}

void XS::ASTTabWidget::CreateClass( QTreeWidgetItem * group )
{
	auto module_ptr = SP_CAST< XE::ASTInfoModule >( GetASTParent( group, ASTType::MODULE ) );

	QString name = "NewClass";
	for ( int i = 1; std::find_if( module_ptr->Classes.begin(), module_ptr->Classes.end(), [&]( const auto & val ) { return val->Name == name.toStdString(); } ) != module_ptr->Classes.end(); i++ )
	{
		name = QString( "NewClass_%1" ).arg( i );
	}

	auto ast = XE::MakeShared< XE::ASTInfoClass >();
	{
		ast->Name = name.toStdString();
		ast->Construct = XE::MakeShared< XE::ASTInfoMethod >();
		{
			ast->Construct->Name = "Construct";
			ast->Construct->Result = XE::MakeShared< XE::ASTInfoType >();
		}
		ast->Destruct = XE::MakeShared< XE::ASTInfoMethod >();
		{
			ast->Destruct->Name = "Destruct";
			ast->Destruct->Result = XE::MakeShared< XE::ASTInfoType >();
		}
	}

	PushUndoCommand( tr( "Create Class " ) + name,
					 [this, ast, group, module_ptr]()
					{
						module_ptr->Classes.push_back( ast );
						ui->tree->setCurrentItem( FillClass( ast, group ) );
					},
					 [this, ast, group, module_ptr]()
					{
						module_ptr->Classes.erase( std::find( module_ptr->Classes.begin(), module_ptr->Classes.end(), ast ) );

						for ( int i = 0; i < group->childCount(); i++ )
						{
							if ( group->child( i )->data( 0, AST_PTR_ROLE ).value<XE::ASTInfoMetaPtr>() == ast )
							{
								delete group->takeChild( i );
								break;
							}
						}
					} );
}

void XS::ASTTabWidget::CreateModule( QTreeWidgetItem * group )
{
	auto module_ptr = SP_CAST< XE::ASTInfoModule >( GetASTParent( group, ASTType::MODULE ) );

	QString name = "NewModule";
	for ( int i = 1; std::find_if( module_ptr->Modules.begin(), module_ptr->Modules.end(), [&]( const auto & val ) { return val->Name == name.toStdString(); } ) != module_ptr->Modules.end(); i++ )
	{
		name = QString( "NewModule_%1" ).arg( i );
	}

	auto ast = XE::MakeShared< XE::ASTInfoModule >();
	ast->Name = name.toStdString();

	PushUndoCommand( tr( "Create Module " ) + name,
					 [this, ast, group, module_ptr]()
	{
		module_ptr->Modules.push_back( ast );
		ui->tree->setCurrentItem( FillModule( ast, group ) );
	},
					 [this, ast, group, module_ptr]()
					{
						module_ptr->Modules.erase( std::find( module_ptr->Modules.begin(), module_ptr->Modules.end(), ast ) );

						for ( int i = 0; i < group->childCount(); i++ )
						{
							if ( group->child( i )->data( 0, AST_PTR_ROLE ).value<XE::ASTInfoMetaPtr>() == ast )
							{
								delete group->takeChild( i );
								break;
							}
						}
					} );
}

void XS::ASTTabWidget::CreateMethod( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateProperty( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateFunction( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateVariable( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateMethodParameter( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateFunctionParameter( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateMethodLocalVariable( QTreeWidgetItem * group )
{

}

void XS::ASTTabWidget::CreateFunctionLocalVariable( QTreeWidgetItem * group )
{

}

XE::ASTInfoMetaPtr XS::ASTTabWidget::GetASTParent( QTreeWidgetItem * item, int parent_t )
{
	while ( item != nullptr )
	{
		int type = item->data( 0, AST_TYPE_ROLE ).toInt();
		if ( type == parent_t )
		{
			return item->data( 0, AST_PTR_ROLE ).value<XE::ASTInfoMetaPtr>();
		}
		item = item->parent();
	}

	if ( parent_t == ASTType::MODULE )
	{
		return _Module;
	}

	return nullptr;
}
