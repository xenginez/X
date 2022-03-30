#include "ObjectsDockWidget.h"

#include "ui_ObjectsDockWidget.h"

#include <QTimer>

REG_WIDGET( XS::ObjectsDockWidget );

namespace TestModule
{
	IMPLEMENT_META_MODULE( TestModule );

	enum class ObjectsDockWidgetEnumTest
	{
		E_0 = 0,
		E_1 = 1 << 1,
		E_2 = 1 << 2,
		E_4 = 1 << 3,
		E_8 = 1 << 4,
	};
	DECL_META_ENUM( TestModule, ObjectsDockWidgetEnumTest );

	class ObjectsDockWidgetClassTest1
	{
	public:
		bool b;
		int i;
		float f;
		double d;
		XE::String s;
		ObjectsDockWidgetEnumTest e;
	};
	DECL_META_CLASS( TestModule, ObjectsDockWidgetClassTest1 );

	class ObjectsDockWidgetClassTest2
	{
	public:
		ObjectsDockWidgetClassTest1 c;
		ObjectsDockWidgetEnumTest f;
	};
	DECL_META_CLASS( TestModule, ObjectsDockWidgetClassTest2 );
}
BEG_META( TestModule::ObjectsDockWidgetEnumTest )
type->Value( "E_0", TestModule::ObjectsDockWidgetEnumTest::E_0 );
type->Value( "E_1", TestModule::ObjectsDockWidgetEnumTest::E_1 );
type->Value( "E_2", TestModule::ObjectsDockWidgetEnumTest::E_2 );
type->Value( "E_4", TestModule::ObjectsDockWidgetEnumTest::E_4 );
type->Value( "E_8", TestModule::ObjectsDockWidgetEnumTest::E_8 );
END_META()
BEG_META( TestModule::ObjectsDockWidgetClassTest1 )
type->Property( "b", &TestModule::ObjectsDockWidgetClassTest1::b );
type->Property( "i", &TestModule::ObjectsDockWidgetClassTest1::i );
type->Property( "f", &TestModule::ObjectsDockWidgetClassTest1::f );
type->Property( "d", &TestModule::ObjectsDockWidgetClassTest1::d );
type->Property( "e", &TestModule::ObjectsDockWidgetClassTest1::e );
type->Property( "s", &TestModule::ObjectsDockWidgetClassTest1::s );
END_META()
BEG_META( TestModule::ObjectsDockWidgetClassTest2 )
type->Property( "c", &TestModule::ObjectsDockWidgetClassTest2::c );
type->Property( "f", &TestModule::ObjectsDockWidgetClassTest2::f )->Attribute( XE::FlagAttribute() );
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
			OnInspectorClicked();
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
		settings.setValue( "splitter_horizontal", ui->splitter->saveState() );
		settings.setValue( "splitter_vertical", ui->splitter_2->saveState() );
	}
	settings.endGroup();
}

void XS::ObjectsDockWidget::Load( QSettings & settings )
{
	DockWidget::Load( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter_2->restoreState( settings.value( "splitter_vertical" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_horizontal" ).toByteArray() );
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

void XS::ObjectsDockWidget::OnInspectorClicked()
{
	XE::Variant var = XE::MakeShared< TestModule::ObjectsDockWidgetClassTest2 >();

	if ( ui->inspector_layout->count() != 0 )
	{
		ui->inspector_layout->removeItem( ui->inspector_layout->itemAt( 0 ) );
	}

	_Inspector = XS::Inspector::Create( XE::MakeShared< XS::ObjectProxy >( var ), this );

	ui->inspector_layout->addWidget( _Inspector );
}
