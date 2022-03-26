#include "LoggerDockWidget.h"

#include "ui_LoggerDockWidget.h"

#include <Skin.h>

XS::LoggerDockWidget::LoggerDockWidget( QWidget * parent /*= nullptr */ )
	:XS::DockWidget( parent ), ui( new Ui::LoggerDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_logger.png" ) );

	setTitleBar( ui->title_bar );

	ui->clear->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_clear.png" ) );
	ui->merge->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_merge.png" ) );
	ui->error->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_error.png" ) );
	ui->warning->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_warning.png" ) );
	ui->info->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_info.png" ) );

	ui->search->addAction( QIcon( "SkinIcons:/images/logger/icon_logger_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	connect( ui->clear, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnClearButtonClicked );
	connect( ui->merge, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnMergeButtonClicked );
	connect( ui->error, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnErrorButtonClicked );
	connect( ui->warning, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnWarningButtonClicked );
	connect( ui->info, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnInfoButtonClicked );
	connect( ui->search, &QLineEdit::editingFinished, this, &XS::LoggerDockWidget::OnSearchEditingFinished );
}

XS::LoggerDockWidget::~LoggerDockWidget()
{
	delete ui;
}

void XS::LoggerDockWidget::OnErrorButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnWarningButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnInfoButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnClearButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnMergeButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnSearchEditingFinished()
{

}
