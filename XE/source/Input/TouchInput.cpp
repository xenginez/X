#include "TouchInput.h"

struct XE::TouchInput::Private
{
	XE::InputState _State;
};

XE::TouchInput::TouchInput()
	: _p( XE::New< Private >() )
{
}

XE::TouchInput::~TouchInput()
{
	XE::Delete( _p );
}

void XE::TouchInput::Startup()
{
}

void XE::TouchInput::Update()
{
}

void XE::TouchInput::Clearup()
{
}

void XE::TouchInput::ClearStatus()
{
}

const XE::InputState & XE::TouchInput::GetState() const
{
	return _p->_State;
}

bool XE::TouchInput::Vibrate( XE::uint32 index, XE::float32 left, XE::float32 right )
{
	return false;
}
