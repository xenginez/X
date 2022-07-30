#include "RenderWidget.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <Windows.h>
#else
#endif

XS::RenderWidget::RenderWidget( QWidget * parent /*= nullptr */ )
	:XS::Widget( parent )
{

}

XS::RenderWidget::~RenderWidget()
{

}

const XE::GraphicsSurfacePtr & XS::RenderWidget::GetSurface() const
{
	return _Surface;
}

void XS::RenderWidget::paintEvent( QPaintEvent * event )
{
	XS::Widget::paintEvent( event );
}

void XS::RenderWidget::showEvent( QShowEvent * event )
{
	XS::Widget::showEvent( event );

	if ( auto graphics = GetFramework()->GetServiceT< XE::GraphicsService >() )
	{
#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
		XE::WindowPtr window = XE::MakeShared< XE::Window >( XE::HandleCast< XE::Window >( static_cast<XE::uint64>( winId() ) ), ( HINSTANCE )::GetModuleHandle( NULL ) );
#else
		XE::WindowPtr window = nullptr; // TODO: 
#endif

		XE::GraphicsSurfaceDescriptor desc = {};
		{
			desc.Label = objectName().toUtf8().constData();
			desc.Window = window;
		}
		_Surface = graphics->CreateSurface( desc );
	}
}

void XS::RenderWidget::closeEvent( QCloseEvent * event )
{
	_Surface = nullptr;

	XS::Widget::closeEvent( event );
}

void XS::RenderWidget::resizeEvent( QResizeEvent * event )
{
	XS::Widget::resizeEvent( event );
}
