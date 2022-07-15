#include "LayoutDialog.h"

#include "ui_SizeDialog.h"
#include "ui_LayoutDialog.h"

#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QMessageBox>

namespace
{
	class QLabelUserData : public QObjectUserData
	{
	public:
		QLabelUserData( int x, int y )
			:pos( x, y )
		{
		}

	public:
		QPoint pos;
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

XS::LayoutDialog::LayoutDialog( QSize size /*= { 1,1 }*/, QWidget * parent /*= nullptr */ )
	:ui( new Ui::LayoutDialog ), _Size( size )
{
	ui->setupUi( this );
	setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

	ui->layout_1->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_1.png" ) );
	ui->layout_4->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_4.png" ) );
	ui->layout_9->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_9.png" ) );
	ui->layout_custom->setIcon( QIcon( "SkinIcons:/images/gamescene/icon_gamescene_layout_custom.png" ) );

	QPushButton * ok_btn = ui->buttonBox->button( QDialogButtonBox::Ok );
	QPushButton * cel_btn = ui->buttonBox->button( QDialogButtonBox::Cancel );

	disconnect( ok_btn );
	disconnect( cel_btn );

	connect( ok_btn, &QPushButton::clicked, [this]() { Rebuild(); if ( _Rects.empty() ) { QMessageBox::warning( this, tr( "error" ), tr( "There are non-rectangles" ), QMessageBox::Yes ); } else { accepted(); } } );
	connect( cel_btn, &QPushButton::clicked, [this]() { rejected(); } );

	connect( ui->layout_1, &QToolButton::clicked, [this]() { OnLayoutSize( { 1, 1 } ); } );
	connect( ui->layout_4, &QToolButton::clicked, [this]() { OnLayoutSize( { 2, 2 } ); } );
	connect( ui->layout_9, &QToolButton::clicked, [this]() { OnLayoutSize( { 3, 3 } ); } );
	connect( ui->layout_custom, &QToolButton::clicked, [this]() { OnLayoutCustomSize(); } );
}

XS::LayoutDialog::~LayoutDialog()
{
	delete ui;
}

void XS::LayoutDialog::OnLayoutSize( QSize size )
{
	_Size = size;

	if ( ( _Size.width() == 1 && _Size.height() == 1 ) || ( _Size.width() == 2 && _Size.height() == 2 ) || ( _Size.width() == 3 && _Size.height() == 3 ) )
	{
		ResizeLayout( false );

		for ( auto & row : _Labels )
		{
			for ( auto item : row )
			{
				QLabelUserData * data = (QLabelUserData *)item->userData( Qt::UserRole + 1 );

				data->prev = data->flag;
				data->flag = RandomColor();
				data->select = false;

				item->setStyleSheet( QString( "background-color: rgba(%1, %2, %3, 100);" ).arg( data->flag.red() ).arg( data->flag.green() ).arg( data->flag.blue() ) );
			}
		}
	}
	else
	{
		ResizeLayout( true );
	}
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
		OnLayoutSize( { ui.col->value(), ui.row->value() } );
	}
}

void XS::LayoutDialog::ResizeLayout( bool select )
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

	_Labels.resize( _Size.width() );
	for ( auto & row : _Labels )
	{
		row.resize( _Size.height() );
	}

	for ( int x = 0; x < _Size.width(); x++ )
	{
		for ( int y = 0; y < _Size.height(); y++ )
		{
			QLabel * label = new QLabel( this );

			label->setObjectName( QString( "label_%1_%2" ).arg( x, 2, 10, QChar( '0' ) ).arg( y, 2, 10, QChar( '0' ) ) );
			label->setAlignment( Qt::AlignCenter );
			label->setStyleSheet( "background-color: rgba(0, 0, 0, 100);" );
			label->setText( QString( "%1-%2" ).arg( x, 2, 10, QChar( '0' ) ).arg( y, 2, 10, QChar( '0' ) ) );
			label->setUserData( Qt::UserRole + 1, new QLabelUserData( x, y ) );

			if ( select )
			{
				label->setMouseTracking( true );
				label->installEventFilter( this );
			}

			ui->gridLayout->addWidget( label, y, x );

			_Labels[x][y] = label;
		}
	}
}

QColor XS::LayoutDialog::RandomColor()
{
	_ColorIdx = _ColorIdx % 25;

	return Colors[_ColorIdx++];
}

void XS::LayoutDialog::Rebuild()
{
	_Rects.clear();

	QMap< unsigned int, QVector< QLabelUserData * > > maps;

	for ( const auto & row : _Labels )
	{
		for ( auto item : row )
		{
			auto data = (QLabelUserData *)item->userData( Qt::UserRole + 1 );

			maps[data->flag.rgba()].push_back( data );
		}
	}

	for ( const auto & it : maps )
	{
		QPoint p1, p2, p3, p4;
		int min_x, max_x, min_y, max_y;

		min_x = it[0]->pos.x();
		max_x = it[0]->pos.x();
		min_y = it[0]->pos.y();
		max_y = it[0]->pos.y();

		for ( auto data : it )
		{
			min_x = std::min( min_x, data->pos.x() );
			max_x = std::max( max_x, data->pos.x() );
			min_y = std::min( min_y, data->pos.y() );
			max_y = std::max( max_y, data->pos.y() );

			if ( data->pos.x() == min_x && data->pos.y() == min_y ) { p1 = data->pos; }
			if ( data->pos.x() == max_x && data->pos.y() == min_y ) { p2 = data->pos; }
			if ( data->pos.x() == min_x && data->pos.y() == max_y ) { p3 = data->pos; }
			if ( data->pos.x() == max_x && data->pos.y() == max_y ) { p4 = data->pos; }
		}

		QPointF center;
		center.setX( ( p1.x() + p2.x() + p3.x() + p4.x() ) / 4.0f );
		center.setY( ( p1.y() + p2.y() + p3.y() + p4.y() ) / 4.0f );

		float dd1 = std::sqrt( std::pow( center.x() - p1.x(), 2 ) + std::pow( center.y() - p1.y(), 2 ) );
		float dd2 = std::sqrt( std::pow( center.x() - p2.x(), 2 ) + std::pow( center.y() - p2.y(), 2 ) );
		float dd3 = std::sqrt( std::pow( center.x() - p3.x(), 2 ) + std::pow( center.y() - p3.y(), 2 ) );
		float dd4 = std::sqrt( std::pow( center.x() - p4.x(), 2 ) + std::pow( center.y() - p4.y(), 2 ) );

		if ( std::abs( dd1 - dd2 ) < std::numeric_limits<float>::epsilon() && std::abs( dd1 - dd3 ) < std::numeric_limits<float>::epsilon() && std::abs( dd1 - dd4 ) < std::numeric_limits<float>::epsilon() )
		{
			_Rects.push_back( QRect( p1.x(), p1.y(), p4.x() - p1.x() + 1, p4.y() - p1.y() + 1 ) );
		}
		else
		{
			_Rects.clear();
			return;
		}
	}
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

void XS::LayoutDialog::showEvent( QShowEvent * e )
{
	QDialog::showEvent( e );

	OnLayoutSize( _Size );
}

const QSize & XS::LayoutDialog::GetRebuildSize() const
{
	return _Size;
}

const QVector< QRect > & XS::LayoutDialog::GetRebuildRects() const
{
	return _Rects;
}
