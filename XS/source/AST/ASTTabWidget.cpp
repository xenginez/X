#include "ASTTabWidget.h"

#include "ui_ASTTabWidget.h"

#include <fstream>

#include <QMenu>
#include <QCursor>
#include <QAbstractItemModel>

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
}

XS::ASTTabWidget::ASTTabWidget( QWidget * parent /*= nullptr */ )
	: Widget( parent ), ui( new Ui::ASTTabWidget )
{
	ui->setupUi( this );

	ui->add->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_add.png" ) );
	ui->tool->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_tool.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setSizes( { 2500, 5000, 2500 } );

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
		ui->detail_title->setText( ast->Name.c_str() );
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
			ui->enum_list->clear();
			for ( const auto & it : _Tree->GetEnums() )
			{
				QListWidgetItem * item = new QListWidgetItem( ui->enum_list );
				{
					item->setText( it->Name.c_str() );
					item->setData( AST_ROLE, QVariant::fromValue( it ) );
					item->setFlags( item->flags() | Qt::ItemIsEditable );
				}
				ui->enum_list->addItem( item );
			}
		}
	}
}

void XS::ASTTabWidget::OnRedo()
{
	if ( GetCommandCount() != 0 )
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
}

void XS::ASTTabWidget::OnUndo()
{
	if ( GetCommandCount() == 0 )
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
