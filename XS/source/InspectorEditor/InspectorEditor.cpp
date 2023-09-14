#include "InspectorEditor.h"
#include "ui_InspectorEditor.h"

#include <QTimer>
REG_WIDGET( XS::InspectorEditor );

XS::InspectorEditor::InspectorEditor( QWidget * parent )
	: XS::ToolEditorWindow( parent ), ui( new Ui::InspectorEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );

	ui->inspector_expand->setIcon( QIcon( "SkinIcons:/images/inspector/icon_inspector_expand.png" ) );
	ui->inspector_collapse->setIcon( QIcon( "SkinIcons:/images/inspector/icon_inspector_collapse.png" ) );

	connect( ui->inspector_expand, &QToolButton::clicked, [this]()
	{
		if( _Inspector != nullptr ) _Inspector->Expand();
	} );
	connect( ui->inspector_collapse, &QToolButton::clicked, [this]()
	{
		if( _Inspector != nullptr ) _Inspector->Collapse();
	} );
	connect( XS::CoreFramework::GetCurrentFramework()->GetSignals(), &Signals::Inspector, this, &XS::InspectorEditor::OnInspector );
}

XS::InspectorEditor::~InspectorEditor()
{
	delete ui;
}

QIcon XS::InspectorEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_inspector.png" );
}

QString XS::InspectorEditor::name()
{
	return tr( "Inspector" );
}

void XS::InspectorEditor::OnInspector( const QString & name, const XE::Variant & val )
{
	if( ui->inspector_layout->count() != 0 )
	{
		ui->inspector_layout->removeItem( ui->inspector_layout->itemAt( 0 ) );
	}

	_Inspector = XS::Inspector::Create( new XS::VariantObjectProxy( val ), this );

	ui->inspector_layout->addWidget( _Inspector );

	ui->inspector_name->setToolTip( name );
	ui->inspector_name->setText( QFontMetrics( ui->inspector_name->font() ).elidedText( name, Qt::ElideRight, ui->inspector_name->width() ) );
}
