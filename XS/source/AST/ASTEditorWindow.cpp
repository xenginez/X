#include "ASTEditorWindow.h"

#include "ui_ASTEditorWindow.h"

#include <QFile>

#include "ASTTabWidget.h"

REG_ASSET( XS::ASTEditor );

namespace
{
	static constexpr const char * UUID_NAME = "AST_UUID";
	static constexpr const char * PATH_NAME = "AST_PATH";
}

XS::ASTEditor::ASTEditor( QObject * parent /*= nullptr */ )
	: XS::AssetEditor( parent )
{

}

XS::ASTEditor::~ASTEditor()
{

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
	return "ast";
}

QUuid XS::ASTEditor::assetCreate( const QFileInfo & path )
{
	XE::ASTInfoModulePtr tree = XE::MakeShared< XE::ASTInfoModule >();
	{
		tree->Name = path.baseName().toStdString();

		std::stringstream json;
		XE::JsonOArchive archive( json );
		archive << tree;

		QFile file( path.absoluteFilePath() );
		if ( file.open( QIODevice::ReadWrite ) )
		{
			std::string data = json.str();
			file.write( data.c_str(), data.size() );
		}
	}

	QUuid uuid = QUuid::createUuid();
	{
		QJsonDocument doc;

		assetDatabase()->Insert( uuid, path, doc );
	}
	return uuid;
}

QIcon XS::ASTEditor::assetIcon( const QUuid & uuid )
{
	return  QIcon( "SkinIcons:/images/icons/icon_asset_ast.svg" );
}

void XS::ASTEditor::assetRemove( const QUuid & uuid )
{
	auto pair = assetDatabase()->Query( uuid );
	if ( pair.first.exists() )
	{
		if ( QFile::remove( pair.first.absoluteFilePath() ) )
		{
			assetDatabase()->Remove( uuid );
		}
	}
}

void XS::ASTEditor::assetRename( const QUuid & uuid, const QFileInfo & old_dir, const QFileInfo & new_dir )
{
	if ( QFile::rename( old_dir.absoluteFilePath(), new_dir.absoluteFilePath() ) )
	{
		assetDatabase()->Update( uuid, new_dir );
	}
}

const QMetaObject * XS::ASTEditor::assetEditor()
{
	return &XS::ASTEditorWindow::staticMetaObject;
}

void XS::ASTEditor::assetBuild( const QUuid & uuid )
{

}


XS::ASTEditorWindow::ASTEditorWindow( QWidget * parent /*= nullptr */ )
	: XS::AssetEditorWindow( parent ), ui( new Ui::ASTEditorWindow ), _Editor( nullptr )
{
	setupUi( ui );
	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_ast.png" ) );
	setTitleBar( ui->title_bar );

	connect( ui->tabWidget, &QTabWidget::tabCloseRequested, this, &XS::ASTEditorWindow::OnTabCloseRequested );
}

XS::ASTEditorWindow::~ASTEditorWindow()
{
	delete ui;
}

void XS::ASTEditorWindow::assetOpen( const QUuid & uuid )
{
	for ( int i = 0; i < ui->tabWidget->count(); ++i )
	{
		if ( ui->tabWidget->widget( i )->property( UUID_NAME ).toUuid() == uuid )
		{
			ui->tabWidget->setCurrentIndex( i );
			return;
		}
	}

	auto path = _Editor->assetDatabase()->QueryPath( uuid );

	if ( path.exists() )
	{
		QDir dir( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string().c_str() );

		XS::ASTTabWidget * tab = new XS::ASTTabWidget( ui->tabWidget );
		{
			tab->setProperty( UUID_NAME, uuid );
			tab->setProperty( PATH_NAME, QVariant::fromValue( path ) );
		}
		ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab( tab, dir.relativeFilePath( path.absoluteFilePath() ) ) );
	}
}

void XS::ASTEditorWindow::SaveLayout( QSettings & settings )
{
	XS::AssetEditorWindow::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.beginWriteArray( "tabs", ui->tabWidget->count() );
		for ( size_t i = 0; i < ui->tabWidget->count(); i++ )
		{
			settings.setArrayIndex( i );

			// qobject_cast<XS::Widget *>( ui->tabWidget->widget( i ) )->SaveLayout( settings );

			settings.setValue( "uuid", ui->tabWidget->widget( i )->property( UUID_NAME ) );
		}
		settings.endArray();
	}
	settings.endGroup();
}

void XS::ASTEditorWindow::LoadLayout( QSettings & settings )
{
	XS::AssetEditorWindow::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		auto sz = settings.beginReadArray( "tabs" );
		for ( size_t i = 0; i < sz; i++ )
		{
			settings.setArrayIndex( i );

			assetOpen( settings.value( "uuid" ).toUuid() );

			// qobject_cast<XS::Widget *>( ui->tabWidget->widget( i ) )->SaveLayout( settings );
		}
		settings.endArray();
	}
	settings.endGroup();
}

void XS::ASTEditorWindow::OnTabCloseRequested( int index )
{
	ui->tabWidget->removeTab( index );
}
