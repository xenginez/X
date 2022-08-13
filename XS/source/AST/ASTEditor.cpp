#include "ASTEditor.h"

#include "ui_ASTEditor.h"

#include <QMenu>
#include <QCursor>

REG_WIDGET( XS::ASTEditor );

namespace
{
	class TreeViewModel : public QAbstractItemModel
	{
	public:
		TreeViewModel( QObject * parent = nullptr );

		~TreeViewModel();

	public:
		QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const override;
		QModelIndex parent( const QModelIndex & index ) const override;
		int rowCount( const QModelIndex & parent = QModelIndex() ) const override;
		int columnCount( const QModelIndex & parent = QModelIndex() ) const override;
		QVariant data( const QModelIndex & index, int role ) const override;
		Qt::ItemFlags flags( const QModelIndex & index ) const override;
		bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() ) override;
	};
}

XS::ASTEditor::ASTEditor( QWidget * parent /*= nullptr */ )
	: XS::AssetEditorWindow( parent ), ui( new Ui::ASTEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );

	ui->add->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_add.png" ) );
	ui->tool->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_tool.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setStretchFactor( 0, 2 );
	ui->splitter->setStretchFactor( 1, 8 );
	ui->splitter->setStretchFactor( 2, 2 );

	connect( ui->add, &QToolButton::clicked, this, &XS::ASTEditor::OnAddToolButtonClicked );
	connect( ui->tool, &QToolButton::clicked, this, &XS::ASTEditor::OnToolToolButtonClicked );
}

XS::ASTEditor::~ASTEditor()
{
	delete ui;
}

QIcon XS::ASTEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_ast.png" );
}

QString XS::ASTEditor::name()
{
	return tr( "AST" );
}

QString XS::ASTEditor::extensionName()
{
	return ".ast";
}

QUuid XS::ASTEditor::assetCreate( const QDir & path )
{
	return {};
}

QIcon XS::ASTEditor::assetIcon( const QUuid & uuid )
{
	return {};
}

void XS::ASTEditor::assetRemove( const QUuid & uuid )
{

}

void XS::ASTEditor::assetOpen( const QUuid & uuid )
{

}

void XS::ASTEditor::SaveLayout( QSettings & settings )
{
	XS::AssetEditorWindow::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter", ui->splitter->saveState() );
	}
	settings.endGroup();
}

void XS::ASTEditor::LoadLayout( QSettings & settings )
{
	XS::AssetEditorWindow::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreState( settings.value( "splitter" ).toByteArray() );
	}
	settings.endGroup();
}

void XS::ASTEditor::OnAddToolButtonClicked( bool clicked )
{
	QMenu menu( this );

	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_module.png" ), "Module", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_enum.png" ), "Enum", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_class.png" ), "Class", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_function.png" ), "Function", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_variable.png" ), "Variable", [this]() {} );

	menu.exec( QCursor::pos() );
}

void XS::ASTEditor::OnToolToolButtonClicked( bool clicked )
{

}
