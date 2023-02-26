#include "CodeEdit.h"

#include <QMenu>

XS::CodeEdit::CodeEdit(QWidget *parent)
	: QTextEdit(parent)
{
	setContextMenuPolicy( Qt::CustomContextMenu );

	connect( this, &QWidget::customContextMenuRequested, this, &CodeEdit::OnCustomContextMenuRequested );
}

XS::CodeEdit::~CodeEdit()
{
}

void XS::CodeEdit::Open( const QString & file )
{

}

void XS::CodeEdit::Save() const
{

}

void XS::CodeEdit::SaveAs( const QString & file ) const
{

}

void XS::CodeEdit::OnCustomContextMenuRequested( const QPoint & pos )
{
	QMenu menu( this );
	{

	}
	menu.exec( QCursor::pos() );
}

void XS::CodeEdit::paintEvent( QPaintEvent * e )
{

	QTextEdit::paintEvent( e );
}

void XS::CodeEdit::closeEvent( QCloseEvent * e )
{

	QTextEdit::closeEvent( e );
}
