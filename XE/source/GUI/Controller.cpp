#include "Controller.h"

IMPLEMENT_META( XE::Controller );

XE::Controller::Controller()
{

}

XE::Controller::~Controller()
{

}

XE::CanvasPtr XE::Controller::GetCanvas() const
{
	return _Canvas.lock();
}

XE::WidgetPtr XE::Controller::GetWidget() const
{
	return _Widget.lock();
}

void XE::Controller::SetWidget( const XE::WidgetPtr & val )
{
	_Widget = val;
}
