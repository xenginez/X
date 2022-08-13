#include "ASTEditor.h"

#include "ui_ASTEditor.h"

#include <QFile>

#include "ASTTabWidget.h"

REG_WIDGET( XS::ASTEditor );

XS::ASTEditor::ASTEditor( QWidget * parent /*= nullptr */ )
	: XS::AssetEditorWindow( parent ), ui( new Ui::ASTEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );
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
	XE::SyntaxTreePtr tree = XE::MakeShared< XE::SyntaxTree >();

	std::stringstream json;
	XE::JsonOArchive archive( json );
	archive << tree;

	QFile file( path.path() );
	if ( file.open( QIODevice::ReadWrite ) )
	{
		std::string data = json.str();
		file.write( data.c_str(), data.size() );
	}

	QUuid uuid = QUuid::createUuid();



	return uuid;
}

QIcon XS::ASTEditor::assetIcon( const QUuid & uuid )
{
	return  QIcon( "SkinIcons:/images/icons/icon_asset_ast.svg" );
}

void XS::ASTEditor::assetRemove( const QUuid & uuid )
{
	auto pair = assetDatabase()->Query( uuid );
	if ( !pair.first.isEmpty() )
	{
		// TODO: other files

		if ( QFile::remove( pair.first.path() ) )
		{
			assetDatabase()->Remove( uuid );
		}
	}
}

void XS::ASTEditor::assetRename( const QUuid & uuid, const QDir & old_dir, const QDir & new_dir )
{
	if ( QFile::rename( old_dir.path(), new_dir.path() ) )
	{
		assetDatabase()->Update( uuid, new_dir );
	}
}

void XS::ASTEditor::assetOpen( const QUuid & uuid )
{
	for ( int i = 0; i < ui->tabWidget->count(); ++i )
	{
		if ( ui->tabWidget->widget( i )->property( "UUID" ).toUuid() == uuid )
		{
			ui->tabWidget->setCurrentIndex( i );
			return;
		}
	}

	auto pair = assetDatabase()->Query( uuid );
	XS::ASTTabWidget * node = new XS::ASTTabWidget( this );
	{
		node->setProperty( "UUID", uuid );
	}
	ui->tabWidget->addTab( node, pair.first.path() );
}

void XS::ASTEditor::SaveLayout( QSettings & settings )
{
	XS::AssetEditorWindow::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.beginWriteArray( "tabs", ui->tabWidget->count() );
		for ( size_t i = 0; i < ui->tabWidget->count(); i++ )
		{
			settings.setArrayIndex( i );
			settings.setValue( "uuid", ui->tabWidget->widget( i )->property( "UUID" ) );
		}
		settings.endArray();
	}
	settings.endGroup();
}

void XS::ASTEditor::LoadLayout( QSettings & settings )
{
	XS::AssetEditorWindow::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		auto sz = settings.beginReadArray( "tabs" );
		for ( size_t i = 0; i < sz; i++ )
		{
			settings.setArrayIndex( i );
			assetOpen( settings.value( "uuid" ).toUuid() );
		}
		settings.endArray();
	}
	settings.endGroup();
}
