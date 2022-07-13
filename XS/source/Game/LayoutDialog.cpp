#include "LayoutDialog.h"

#include "ui_SizeDialog.h"
#include "ui_LayoutDialog.h"

#include <QGridLayout>
#include <QMouseEvent>

namespace
{

	class QLabelUserData : public QObjectUserData
	{
	public:
		QLabelUserData( int x, int y )
			:x( x ), y( y )
		{
		}

	public:
		int x, y;
		bool select = false;
		QColor flag = Qt::black;
		QColor prev = Qt::black;
	};

	static const QColor Colors[25] =
	{
		{  138,  72,  126  },
		{  128,  182,  144  },
		{  94,  97,  191  },
		{  171,  146,  178  },
		{  115,  98,  111  },
		{  71,  89,  184  },
		{  107,  157,  188  },
		{  109,  104,  89  },
		{  119,  180,  69  },
		{  179,  173,  127  },
		{  84,  147,  91  },
		{  129,  122,  80  },
		{  92,  179,  153  },
		{  178,  121,  156  },
		{  135,  113,  75  },
		{  107,  122,  149  },
		{  158,  91,  141  },
		{  74,  143,  86  },
		{  180,  149,  115  },
		{  138,  147,  126  },
		{  152,  163,  152  },
		{  148,  123,  168  },
		{  183,  191,  120  },
		{  138,  67,  124  },
		{  147,  116,  174  },
	};
}

XS::LayoutDialog::LayoutDialog( QWidget * parent /*= nullptr */ )
	:ui( new Ui::LayoutDialog )
{
	ui->setupUi( this );
	setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

	ui->layout_1->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_1.png" ) );
	ui->layout_4->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_4.png" ) );
	ui->layout_9->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_9.png" ) );
	ui->layout_custom->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_custom.png" ) );

	connect( ui->layout_1, &QToolButton::clicked, [this]() { OnLayoutSize( { 1, 1 } ); } );
	connect( ui->layout_4, &QToolButton::clicked, [this]() { OnLayoutSize( { 2, 2 } ); } );
	connect( ui->layout_9, &QToolButton::clicked, [this]() { OnLayoutSize( { 3, 3 } ); } );
	connect( ui->layout_custom, &QToolButton::clicked, [this]() { OnLayoutCustomSize(); } );
}

XS::LayoutDialog::~LayoutDialog()
{

}

void XS::LayoutDialog::OnLayoutSize( QSize size )
{

}

void XS::LayoutDialog::OnLayoutCustomSize()
{
	QDialog dialog( this );
	Ui::SizeDialog ui;
	ui.setupUi( &dialog );
	dialog.setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

	ui.col->setValue( _Size.width() );
	ui.row->setValue( _Size.height() );

	if ( dialog.exec() == QDialog::Accepted )
	{
		_Size.setWidth( ui.col->value() );
		_Size.setHeight( ui.row->value() );

		ResizeLayout();
	}
}

void XS::LayoutDialog::ResizeLayout()
{
	for ( auto & row : _Labels )
	{
		for ( auto item : row )
		{
			item->deleteLater();
		}
		row.clear();
	}
	_Labels.clear();

	_Labels.resize( _Size.height() );
	for ( auto & row : _Labels )
	{
		row.resize( _Size.width() );
	}

	for ( int x = 0; x < _Size.height(); x++ )
	{
		for ( int y = 0; y < _Size.width(); y++ )
		{
			QLabel * label = new QLabel( this );

			label->setObjectName( QString( "label_%1_%2" ).arg( x, 2, 10, QChar( '0' ) ).arg( y, 2, 10, QChar( '0' ) ) );
			label->setMouseTracking( true );
			label->setAlignment( Qt::AlignCenter );
			label->setStyleSheet( "background-color: rgba(0, 0, 0, 100);" );
			label->setText( QString( "%1-%2" ).arg( x, 2, 10, QChar( '0' ) ).arg( y, 2, 10, QChar( '0' ) ) );
			label->setUserData( Qt::UserRole + 1, new QLabelUserData( x, y ) );
			label->installEventFilter( this );

			ui->gridLayout->addWidget( label, x, y );

			_Labels[x][y] = label;
		}
	}
}

QColor XS::LayoutDialog::RandomColor()
{
	_ColorIdx = _ColorIdx % 25;

	return Colors[_ColorIdx++];
}

bool XS::LayoutDialog::eventFilter( QObject * o, QEvent * e )
{
	if ( o->objectName().startsWith( "label" ) )
	{
		if ( e->type() == QEvent::MouseButtonPress )
		{
			QMouseEvent * mouse = (QMouseEvent *)e;

			_MousePos = mouse->globalPos();
			_MousePress = true;
			_MouseColor = RandomColor();

			return true;
		}
		else if ( e->type() == QEvent::MouseButtonRelease )
		{
			QMouseEvent * mouse = (QMouseEvent *)e;

			_MousePress = false;
			for ( auto & row : _Labels )
			{
				for ( auto item : row )
				{
					QLabelUserData * data = (QLabelUserData *)item->userData( Qt::UserRole + 1 );

					data->select = false;
					data->prev = data->flag;
				}
			}

			return true;
		}
		else if ( e->type() == QEvent::MouseMove && _MousePress )
		{
			QMouseEvent * mouse = (QMouseEvent *)e;
			auto pos = mouse->globalPos();

			int top = std::min( pos.y(), _MousePos.y() );
			int left = std::min( pos.x(), _MousePos.x() );
			int bottom = std::max( pos.y(), _MousePos.y() );
			int right = std::max( pos.x(), _MousePos.x() );

			QRect mouse_rect( QPoint( left, top ), QSize( right - left, bottom - top ) );

			for ( auto & row : _Labels )
			{
				for ( auto item : row )
				{
					QRect item_rect( item->mapToGlobal( QPoint( 0, 0 ) ), item->size() );

					QLabelUserData * data = (QLabelUserData *)item->userData( Qt::UserRole + 1 );

					if ( !data->select && mouse_rect.intersects( item_rect ) )
					{
						data->prev = data->flag;
						data->flag = _MouseColor;
						data->select = true;

						item->setStyleSheet( QString( "background-color: rgba(%1, %2, %3, 100);" ).arg( data->flag.red() ).arg( data->flag.green() ).arg( data->flag.blue() ) );
					}
					else if( data->select && !mouse_rect.intersects( item_rect ) )
					{
						data->select = false;
						data->flag = data->prev;

						item->setStyleSheet( QString( "background-color: rgba(%1, %2, %3, 100);" ).arg( data->flag.red() ).arg( data->flag.green() ).arg( data->flag.blue() ) );
					}
				}
			}
		}
	}

	return QDialog::eventFilter( o, e );
}
