#include "ASTTabWidget.h"

#include "ui_ASTTabWidget.h"

#include <fstream>

#include <QMenu>
#include <QCursor>
#include <QItemDelegate>

Q_DECLARE_METATYPE( XE::ASTMetaPtr );
Q_DECLARE_METATYPE( XE::ASTEnumPtr );
Q_DECLARE_METATYPE( XE::ASTClassPtr );
Q_DECLARE_METATYPE( XE::ASTModulePtr );
Q_DECLARE_METATYPE( XE::ASTMethodPtr );
Q_DECLARE_METATYPE( XE::ASTPropertyPtr );
Q_DECLARE_METATYPE( XE::ASTFunctionPtr );
Q_DECLARE_METATYPE( XE::ASTVariablePtr );

namespace
{
	static constexpr int MODULE_INDEX = 0;
	static constexpr int ENUM_INDEX = 1;
	static constexpr int CLASS_INDEX = 2;
	static constexpr int METHOD_INDEX = 3;
	static constexpr int PROPERTY_INDEX = 4;
	static constexpr int FUNCTION_INDEX = 5;
	static constexpr int VARIABLE_INDEX = 6;
	static constexpr int AST_ROLE = Qt::UserRole + 1;
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

	class TreeItemDelegate : public QItemDelegate
	{
	public:
		TreeItemDelegate( QObject * parent = nullptr )
			:QItemDelegate( parent )
		{

		}

		QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const override
		{
			QSize size = QItemDelegate::sizeHint( option, index );

			if ( ( index.model()->flags( index ) & Qt::ItemFlag::ItemIsEditable ) == 0 )
			{
				size.setHeight( size.height() * 2 );
			}

			return size;
		}
	};
}

XS::ASTTabWidget::ASTTabWidget( QWidget * parent /*= nullptr */ )
	: Widget( parent ), ui( new Ui::ASTTabWidget )
{
	ui->setupUi( this );

	ui->add->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_add.png" ) );
	ui->tool->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_tool.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setSizes( { 2500, 5000, 2500 } );
	ui->tree->setItemDelegate( new TreeItemDelegate( this ) );

	connect( ui->add, &QToolButton::clicked, this, &XS::ASTTabWidget::OnAddToolButtonClicked );
	connect( ui->tool, &QToolButton::clicked, this, &XS::ASTTabWidget::OnToolToolButtonClicked );
	connect( ui->enum_list, &QListWidget::itemChanged, this, &XS::ASTTabWidget::OnEnumListItemChanged );
	connect( ui->enum_list, &QListWidget::itemDoubleClicked, this, &XS::ASTTabWidget::OnEnumListItemDoubleClicked );
}

XS::ASTTabWidget::~ASTTabWidget()
{

}

void XS::ASTTabWidget::OnAddToolButtonClicked( bool clicked )
{
	QMenu menu( this );

	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_enum.png" ), "Enum", [this]()
	{
		XE::String name = "New Enum";
		for ( size_t i = 1; std::find_if( _Tree->GetEnums().begin(), _Tree->GetEnums().end(), [&]( const auto & val ) { return val->Name == name; } ) != _Tree->GetEnums().end(); ++i )
		{
			name = "New Enum " + XE::ToString( i );
		}

		int toolBoxCurrentIndex = ui->toolBox->currentIndex();
		auto enum_listCurrentIndex = ui->enum_list->currentIndex();

		PushUndoCommand( "Create Enum",
						 [&]()
						{
							XE::ASTEnumPtr _enum = XE::MakeShared< XE::ASTEnum >();
							{
								_enum->Name = name;
							}
							_Tree->AddEnum( _enum );

							QListWidgetItem * item = new QListWidgetItem( ui->enum_list );
							{
								item->setText( name.c_str() );
								item->setData( AST_ROLE, QVariant::fromValue( _enum ) );
								item->setFlags( item->flags() | Qt::ItemIsEditable );
								ui->enum_list->addItem( item );

								ui->toolBox->setCurrentIndex( ENUM_INDEX );

								ui->enum_list->setCurrentItem( item );
								ui->enum_list->editItem( item );
							}
						},
						 [&]()
						{
							auto enums = _Tree->GetEnums();
							auto it = std::find_if( enums.begin(), enums.end(), [name]( const auto & it ) { return it->Name == name; } );
							if ( it != enums.end() ) enums.erase( it );
							_Tree->SetEnums( enums );

							auto items = ui->enum_list->findItems( name.c_str(), Qt::MatchExactly );
							for ( auto it : items )
							{
								auto item = ui->enum_list->takeItem( ui->enum_list->row( it ) );
								if ( item != nullptr ) delete item;
							}

							ui->enum_list->setCurrentIndex( enum_listCurrentIndex );
							ui->toolBox->setCurrentIndex( toolBoxCurrentIndex );
						} );
	} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_class.png" ), "Class", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_module.png" ), "Module", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_function.png" ), "Function", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_variable.png" ), "Variable", [this]() {} );

	menu.exec( QCursor::pos() );
}

void XS::ASTTabWidget::OnToolToolButtonClicked( bool clicked )
{

}

void XS::ASTTabWidget::OnEnumListItemChanged( QListWidgetItem * item )
{
	auto ast = item->data( AST_ROLE ).value<XE::ASTEnumPtr>();
	if ( ast && ast->Name != item->text().toStdString() )
	{
		PushUndoCommand( QString( R"(Enum Name '%1' -> '%2')" ).arg( ast->Name.c_str() ).arg( item->text() ),
						 [ast, item, text = item->text()]() { ast->Name = text.toStdString(); },
						 [ast, item, text = ast->Name]() { ast->Name = text; item->setText( text.c_str() ); } );
	}
}

void XS::ASTTabWidget::OnEnumListItemDoubleClicked( QListWidgetItem * item )
{
	auto ast = item->data( AST_ROLE ).value<XE::ASTEnumPtr>();
	if ( ast )
	{
		if ( _Inspector != nullptr )
		{
			_Inspector->deleteLater();
		}

		XS::ObjectProxy * proxy = new XS::VariantObjectProxy( ast );
		_Inspector = XS::Inspector::Create( proxy, ui->detail );
		ui->detail_title->setText( QString( "Enum - %1" ).arg( ast->Name.c_str() ) );
		ui->detail_layout->addWidget( _Inspector );
	}
}

void XS::ASTTabWidget::showEvent( QShowEvent * event )
{
	XS::Widget::showEvent( event );

	auto path = property( PATH_NAME ).value<QFileInfo>();

	std::ifstream ifs( path.absoluteFilePath().toStdString() );
	if ( ifs.is_open() )
	{
		XE::JsonIArchive archive( ifs );

		archive >> _Tree;

		if ( _Tree )
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

	Rename();
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

	Rename();
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
	archive << _Tree;

	QFile file( path.absoluteFilePath() );
	if ( file.open( QIODevice::ReadWrite ) )
	{
		std::string data = json.str();
		file.write( data.c_str(), data.size() );
	}
	else
	{
		//XE_ERROR( "{%1} file open failure", path.absoluteFilePath().toStdString() );
	}
}

void XS::ASTTabWidget::Rename()
{
	for ( int i = 0; i < ui->enum_list->count(); i++ )
	{
		auto item = ui->enum_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTEnumPtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->class_list->count(); i++ )
	{
		auto item = ui->class_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTClassPtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->module_list->count(); i++ )
	{
		auto item = ui->module_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTModulePtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->method_list->count(); i++ )
	{
		auto item = ui->method_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTMethodPtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->property_list->count(); i++ )
	{
		auto item = ui->property_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTPropertyPtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->function_list->count(); i++ )
	{
		auto item = ui->function_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTFunctionPtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
	for ( int i = 0; i < ui->variable_list->count(); i++ )
	{
		auto item = ui->variable_list->item( i );
		auto ast = item->data( AST_ROLE ).value<XE::ASTVariablePtr>();

		if ( item->text() != ast->Name.c_str() )
		{
			item->setText( QString::fromUtf8( ast->Name.c_str() ) );
		}
	}
}

void XS::ASTTabWidget::FillTree()
{
	ui->tree->clear();

	QTreeWidgetItem * item = new QTreeWidgetItem( ui->tree );
	{
		item->setText( 0, QString::fromUtf8( _Tree->GetName().c_str() ) );
		item->setIcon( 0, QIcon( TREE_ICON ) );
		ui->tree->addTopLevelItem( item );
		auto font = item->font( 0 ); font.setBold( true ); item->setFont( 0, font );
	}

	QTreeWidgetItem * enum_item = new QTreeWidgetItem( item );
	{
		enum_item->setText( 0, tr( "Enum" ) );
		enum_item->setIcon( 0, QIcon( ENUM_ICON ) );
		auto font = enum_item->font( 0 ); font.setBold( true ); enum_item->setFont( 0, font );

		for ( const auto & it : _Tree->GetEnums() )
		{
			FillEnum( it, enum_item );
		}
	}

	QTreeWidgetItem * class_item = new QTreeWidgetItem( item );
	{
		class_item->setText( 0, tr( "Class" ) );
		class_item->setIcon( 0, QIcon( CLASS_ICON ) );
		auto font = class_item->font( 0 ); font.setBold( true ); class_item->setFont( 0, font );

		for ( const auto & it : _Tree->GetClasses() )
		{
			FillClass( it, class_item );
		}
	}

	QTreeWidgetItem * module_item = new QTreeWidgetItem( item );
	{
		module_item->setText( 0, tr( "Module" ) );
		module_item->setIcon( 0, QIcon( MODULE_ICON ) );
		auto font = module_item->font( 0 ); font.setBold( true ); module_item->setFont( 0, font );

		for ( const auto & it : _Tree->GetModules() )
		{
			FillModule( it, module_item );
		}
	}

	QTreeWidgetItem * function_item = new QTreeWidgetItem( item );
	{
		function_item->setText( 0, tr( "Function" ) );
		function_item->setIcon( 0, QIcon( FUNCTION_ICON ) );
		auto font = function_item->font( 0 ); font.setBold( true ); function_item->setFont( 0, font );

		for ( const auto & it : _Tree->GetFunctions() )
		{
			FillFunction( it, function_item );
		}
	}

	QTreeWidgetItem * variable_item = new QTreeWidgetItem( item );
	{
		variable_item->setText( 0, tr( "Variable" ) );
		variable_item->setIcon( 0, QIcon( VARIABLE_ICON ) );
		auto font = variable_item->font( 0 ); font.setBold( true ); variable_item->setFont( 0, font );

		for ( const auto & it : _Tree->GetVariables() )
		{
			FillVariable( it, variable_item );
		}
	}
}

void XS::ASTTabWidget::FillEnum( const XE::ASTEnumPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setIcon( 0, QIcon( ENUM_ICON ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable );
	}

	for ( const auto & it : ast->Elements )
	{
		QTreeWidgetItem * element_item = new QTreeWidgetItem( item );
		{
			element_item->setText( 0, QString::fromUtf8( it.c_str() ) );
			element_item->setIcon( 0, QIcon( ENUM_ICON ) );
			element_item->setFlags( item->flags() | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable );
		}
	}
}

void XS::ASTTabWidget::FillClass( const XE::ASTClassPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setIcon( 0, QIcon( CLASS_ICON ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable );
	}

	FillMethod( ast->Construct, item );
	FillMethod( ast->Destruct, item );

	QTreeWidgetItem * method_item = new QTreeWidgetItem( parent );
	{
		method_item->setText( 0, tr( "Method" ) );
		method_item->setIcon( 0, QIcon( METHOD_ICON ) );
		auto font = method_item->font( 0 ); font.setBold( true ); method_item->setFont( 0, font );

		for ( const auto & it : ast->Methods )
		{
			FillMethod( it, method_item );
		}
	}

	QTreeWidgetItem * property_item = new QTreeWidgetItem( parent );
	{
		property_item->setText( 0, tr( "Property" ) );
		property_item->setIcon( 0, QIcon( METHOD_ICON ) );
		auto font = property_item->font( 0 ); font.setBold( true ); property_item->setFont( 0, font );

		for ( const auto & it : ast->Propertys )
		{
			FillProperty( it, property_item );
		}
	}
}

void XS::ASTTabWidget::FillModule( const XE::ASTModulePtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setIcon( 0, QIcon( MODULE_ICON ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable );
	}

	QTreeWidgetItem * enum_item = new QTreeWidgetItem( item );
	{
		enum_item->setText( 0, tr( "Enum" ) );
		enum_item->setIcon( 0, QIcon( ENUM_ICON ) );
		auto font = enum_item->font( 0 ); font.setBold( true ); enum_item->setFont( 0, font );

		for ( const auto & it : ast->Enums )
		{
			FillEnum( it, enum_item );
		}
	}
	QTreeWidgetItem * class_item = new QTreeWidgetItem( item );
	{
		class_item->setText( 0, tr( "Class" ) );
		class_item->setIcon( 0, QIcon( CLASS_ICON ) );
		auto font = class_item->font( 0 ); font.setBold( true ); class_item->setFont( 0, font );

		for ( const auto & it : ast->Clsses )
		{
			FillClass( it, class_item );
		}
	}
	QTreeWidgetItem * module_item = new QTreeWidgetItem( item );
	{
		module_item->setText( 0, tr( "Module" ) );
		module_item->setIcon( 0, QIcon( MODULE_ICON ) );
		auto font = module_item->font( 0 ); font.setBold( true ); module_item->setFont( 0, font );

		for ( const auto & it : ast->Modules )
		{
			FillModule( it, module_item );
		}
	}
	QTreeWidgetItem * function_item = new QTreeWidgetItem( item );
	{
		function_item->setText( 0, tr( "Function" ) );
		function_item->setIcon( 0, QIcon( FUNCTION_ICON ) );
		auto font = function_item->font( 0 ); font.setBold( true ); function_item->setFont( 0, font );

		for ( const auto & it : ast->Functions )
		{
			FillFunction( it, function_item );
		}
	}
	QTreeWidgetItem * variable_item = new QTreeWidgetItem( item );
	{
		variable_item->setText( 0, tr( "Variable" ) );
		variable_item->setIcon( 0, QIcon( VARIABLE_ICON ) );
		auto font = variable_item->font( 0 ); font.setBold( true ); variable_item->setFont( 0, font );

		for ( const auto & it : ast->Variables )
		{
			FillVariable( it, variable_item );
		}
	}
}

void XS::ASTTabWidget::FillMethod( const XE::ASTMethodPtr & ast, QTreeWidgetItem * parent )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( parent );
	{
		item->setText( 0, QString::fromUtf8( ast->Name.c_str() ) );
		item->setIcon( 0, QIcon( METHOD_ICON ) );
		item->setFlags( item->flags() | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEditable );
	}

	QTreeWidgetItem * parameter_item = new QTreeWidgetItem( item );
	{
		parameter_item->setText( 0, tr( "Parameter" ) );
		parameter_item->setIcon( 0, QIcon( ENUM_ICON ) );
		auto font = parameter_item->font( 0 ); font.setBold( true ); parameter_item->setFont( 0, font );

		for ( const auto & it : ast->Parameters )
		{
			// TODO: 
		}
	}

	QTreeWidgetItem * local_item = new QTreeWidgetItem( item );
	{
		local_item->setText( 0, tr( "LocalVariable" ) );
		local_item->setIcon( 0, QIcon( ENUM_ICON ) );
		auto font = local_item->font( 0 ); font.setBold( true ); local_item->setFont( 0, font );

		for ( const auto & it : ast->LocalVariables )
		{
			// TODO: 
		}
	}
}

void XS::ASTTabWidget::FillProperty( const XE::ASTPropertyPtr & ast, QTreeWidgetItem * parent )
{

}

void XS::ASTTabWidget::FillFunction( const XE::ASTFunctionPtr & ast, QTreeWidgetItem * parent )
{

}

void XS::ASTTabWidget::FillVariable( const XE::ASTVariablePtr & ast, QTreeWidgetItem * parent )
{

}
