#include "ASTTabWidget.h"

#include "ui_ASTTabWidget.h"

#include <QMenu>
#include <QCursor>
#include <QAbstractItemModel>

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

XS::ASTTabWidget::ASTTabWidget( QWidget * parent /*= nullptr */ )
	: QWidget( parent ), ui( new Ui::ASTTabWidget )
{
	ui->setupUi( this );

	ui->add->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_add.png" ) );
	ui->tool->setIcon( QIcon( "SkinIcons:/images/ast/icon_ast_tool.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/ast/icon_ast_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setSizes( { 2500, 5000,2500 } );

	connect( ui->add, &QToolButton::clicked, this, &XS::ASTTabWidget::OnAddToolButtonClicked );
	connect( ui->tool, &QToolButton::clicked, this, &XS::ASTTabWidget::OnToolToolButtonClicked );
}

XS::ASTTabWidget::~ASTTabWidget()
{

}

void XS::ASTTabWidget::OnAddToolButtonClicked( bool clicked )
{
	QMenu menu( this );

	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_module.png" ), "Module", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_enum.png" ), "Enum", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_class.png" ), "Class", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_function.png" ), "Function", [this]() {} );
	menu.addAction( QIcon( "SkinIcons:/images/ast/icon_ast_variable.png" ), "Variable", [this]() {} );

	menu.exec( QCursor::pos() );
}

void XS::ASTTabWidget::OnToolToolButtonClicked( bool clicked )
{

}
