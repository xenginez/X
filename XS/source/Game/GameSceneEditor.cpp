#include "GameSceneEditor.h"

#include "ui_GameSceneEditor.h"

#include <QComboBox>

#include "LayoutDialog.h"

REG_WIDGET( XS::GameSceneEditor );

XS::GameSceneEditor::GameSceneEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::GameSceneEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_game.png" ) );

	setTitleBar( ui->title_bar );

	ui->layout->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout.png" ) );
	connect( ui->layout, &QToolButton::clicked, this, &GameSceneEditor::OnLayoutClicked );
}

XS::GameSceneEditor::~GameSceneEditor()
{
	delete ui;
}

WId XS::GameSceneEditor::display() const
{
	return ui->display->winId();
}

void XS::GameSceneEditor::OnLayoutClicked( bool checked /*= false */ )
{
	XS::LayoutDialog dialog( this );

	dialog.exec();
}

void XS::GameSceneEditor::OnGameSceneFpsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnGameSceneAudioClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnGameSceneToolsClicked( bool checked /*= false */ )
{

}

void XS::GameSceneEditor::OnGameSceneRatioCurrentIndexChanged( int index )
{

}

void XS::GameSceneEditor::OnGameSceneCameraCurrentIndexChanged( int index )
{

}

QWidget * XS::GameSceneEditor::CreateGameScene()
{
	QWidget * GameSceneEditor = new QWidget( this );

	QVBoxLayout * verticalLayout;
	QWidget * title_bar;
	QHBoxLayout * horizontalLayout;
	QComboBox * camera;
	QComboBox * ratio;
	QToolButton * audio;
	QToolButton * fps;
	QToolButton * tools;
	QSpacerItem * horizontalSpacer;
	QWidget * display;

	GameSceneEditor->setObjectName( QString::fromUtf8( "GameScene" ) );
	GameSceneEditor->resize( 972, 655 );
	verticalLayout = new QVBoxLayout( GameSceneEditor );
	verticalLayout->setSpacing( 0 );
	verticalLayout->setObjectName( QString::fromUtf8( "verticalLayout" ) );
	verticalLayout->setContentsMargins( 0, 0, 0, 0 );
	title_bar = new QWidget( GameSceneEditor );
	title_bar->setObjectName( QString::fromUtf8( "title_bar" ) );
	QSizePolicy sizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
	sizePolicy.setHorizontalStretch( 0 );
	sizePolicy.setVerticalStretch( 0 );
	sizePolicy.setHeightForWidth( title_bar->sizePolicy().hasHeightForWidth() );
	title_bar->setSizePolicy( sizePolicy );
	title_bar->setMinimumSize( QSize( 0, 20 ) );
	title_bar->setMaximumSize( QSize( 16777215, 20 ) );
	horizontalLayout = new QHBoxLayout( title_bar );
	horizontalLayout->setSpacing( 3 );
	horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
	horizontalLayout->setContentsMargins( 0, 0, 0, 0 );
	camera = new QComboBox( title_bar );
	camera->setObjectName( QString::fromUtf8( "camera" ) );
	QSizePolicy sizePolicy1( QSizePolicy::Fixed, QSizePolicy::Fixed );
	sizePolicy1.setHorizontalStretch( 0 );
	sizePolicy1.setVerticalStretch( 0 );
	sizePolicy1.setHeightForWidth( camera->sizePolicy().hasHeightForWidth() );
	camera->setSizePolicy( sizePolicy1 );
	camera->setMinimumSize( QSize( 100, 0 ) );
	camera->setMaximumSize( QSize( 100, 16777215 ) );

	horizontalLayout->addWidget( camera );

	ratio = new QComboBox( title_bar );
	ratio->addItem( QString() );
	ratio->addItem( QString() );
	ratio->addItem( QString() );
	ratio->addItem( QString() );
	ratio->addItem( QString() );
	ratio->addItem( QString() );
	ratio->setObjectName( QString::fromUtf8( "ratio" ) );
	sizePolicy1.setHeightForWidth( ratio->sizePolicy().hasHeightForWidth() );
	ratio->setSizePolicy( sizePolicy1 );
	ratio->setMinimumSize( QSize( 150, 0 ) );
	ratio->setMaximumSize( QSize( 150, 16777215 ) );

	horizontalLayout->addWidget( ratio );

	audio = new QToolButton( title_bar );
	audio->setObjectName( QString::fromUtf8( "audio" ) );
	audio->setMinimumSize( QSize( 20, 20 ) );
	audio->setMaximumSize( QSize( 20, 20 ) );
	audio->setCheckable( true );

	horizontalLayout->addWidget( audio );

	fps = new QToolButton( title_bar );
	fps->setObjectName( QString::fromUtf8( "fps" ) );
	fps->setMinimumSize( QSize( 20, 20 ) );
	fps->setMaximumSize( QSize( 20, 20 ) );
	fps->setCheckable( true );

	horizontalLayout->addWidget( fps );

	tools = new QToolButton( title_bar );
	tools->setObjectName( QString::fromUtf8( "tools" ) );
	tools->setMinimumSize( QSize( 20, 20 ) );
	tools->setMaximumSize( QSize( 20, 20 ) );

	horizontalLayout->addWidget( tools );

	horizontalSpacer = new QSpacerItem( 528, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

	horizontalLayout->addItem( horizontalSpacer );


	verticalLayout->addWidget( title_bar );

	display = new QWidget( GameSceneEditor );
	display->setObjectName( QString::fromUtf8( "display" ) );

	verticalLayout->addWidget( display );


	fps->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_fps.png" ) );
	audio->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_audio.png" ) );
	tools->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_tools.png" ) );

	connect( fps, &QToolButton::clicked, this, &GameSceneEditor::OnGameSceneFpsClicked );
	connect( audio, &QToolButton::clicked, this, &GameSceneEditor::OnGameSceneAudioClicked );
	connect( tools, &QToolButton::clicked, this, &GameSceneEditor::OnGameSceneToolsClicked );
	connect( ratio, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneEditor::OnGameSceneRatioCurrentIndexChanged );
	connect( camera, QOverload<int>::of( &QComboBox::currentIndexChanged ), this, &GameSceneEditor::OnGameSceneCameraCurrentIndexChanged );

	return GameSceneEditor;
}
