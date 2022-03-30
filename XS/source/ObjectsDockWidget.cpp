#include "ObjectsDockWidget.h"

#include "ui_ObjectsDockWidget.h"

#include <QTimer>

REG_WIDGET( XS::ObjectsDockWidget );

namespace TestModule
{
	IMPLEMENT_META_MODULE( TestModule );

	class TestClass
	{
	public:
		bool b;
	};
	DECL_META_CLASS( TestModule, TestClass );
}

BEG_META( TestModule::TestClass )
type->Property( "b", &TestModule::TestClass::b );
END_META()

XS::ObjectsDockWidget::ObjectsDockWidget( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::ObjectsDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_objects.png" ) );

	setTitleBar( new QWidget( this ) );

	ui->logic_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_logic.png" ) );
	ui->logic_add->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_add.png" ) );
	ui->logic_type->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_type.png" ) );
	ui->logic_expand->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_expand.png" ) );
	ui->logic_collapse->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_collapse.png" ) );
	ui->logic_search->addAction( QIcon( "SkinIcons:/images/objects/icon_objects_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->render_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_render.png" ) );
	ui->render_add->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_add.png" ) );
	ui->render_type->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_type.png" ) );
	ui->render_expand->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_expand.png" ) );
	ui->render_collapse->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_collapse.png" ) );
	ui->render_search->addAction( QIcon( "SkinIcons:/images/objects/icon_objects_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->inspector_icon->setPixmap( QPixmap( "SkinIcons:/images/objects/icon_objects_inspector.png" ) );
	ui->inspector_expand->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_expand.png" ) );
	ui->inspector_collapse->setIcon( QIcon( "SkinIcons:/images/objects/icon_objects_collapse.png" ) );

	connect( ui->logic_tree, &QTreeWidget::itemClicked, this, &ObjectsDockWidget::OnLogicTreeWidgetItemClicked );
	connect( ui->render_tree, &QTreeWidget::itemClicked, this, &ObjectsDockWidget::OnRenderTreeWidgetItemClicked );

	connect( ui->inspector_expand, &QToolButton::clicked, [this]() { if ( _Inspector != nullptr ) _Inspector->Expand(); } );
	connect( ui->inspector_collapse, &QToolButton::clicked, [this]() { if ( _Inspector != nullptr ) _Inspector->Collapse(); } );

	QTimer::singleShot( 1000, [this]()
		{
			TestModule::TestClass cls;

			cls.b = true;

			OnInspectorClicked( cls );
		} );
}

XS::ObjectsDockWidget::~ObjectsDockWidget()
{
	delete ui;
}

void XS::ObjectsDockWidget::Save( QSettings & settings )
{
	DockWidget::Save( settings );
	
	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_horizontal_geometry", ui->splitter->saveGeometry() );
		settings.setValue( "splitter_vertical_geometry", ui->splitter_2->saveGeometry() );
		settings.setValue( "splitter_horizontal_state", ui->splitter->saveState() );
		settings.setValue( "splitter_vertical_state", ui->splitter_2->saveState() );
	}
	settings.endGroup();
}

void XS::ObjectsDockWidget::Load( QSettings & settings )
{
	DockWidget::Load( settings );
	
	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreGeometry( settings.value( "splitter_horizontal_geometry" ).toByteArray() );
		ui->splitter_2->restoreGeometry( settings.value( "splitter_vertical_geometry" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_horizontal_state" ).toByteArray() );
		ui->splitter_2->restoreState( settings.value( "splitter_vertical_state" ).toByteArray() );
	}
	settings.endGroup();
}

void XS::ObjectsDockWidget::OnLogicTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->render_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::ObjectsDockWidget::OnRenderTreeWidgetItemClicked( QTreeWidgetItem * item, int column )
{
	ui->logic_tree->setCurrentItem( nullptr );
	ui->inspector_name->setText( item->text( 0 ) );

}

void XS::ObjectsDockWidget::OnInspectorClicked( const XE::Variant & val )
{
	if ( ui->inspector_layout->count() != 0 )
	{
		ui->inspector_layout->removeItem( ui->inspector_layout->itemAt( 0 ) );
	}

	_Inspector = XS::Inspector::Create( XE::MakeShared< XS::ObjectProxy >( val ), this );

	ui->inspector_layout->addWidget( _Inspector );
}
