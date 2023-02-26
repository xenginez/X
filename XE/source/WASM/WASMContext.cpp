#include "WASMContext.h"

void XE::WASMContext::Push( const XE::WASMValue & val )
{
	_ValueStack.push_back( val );
}

XE::WASMValue & XE::WASMContext::Top()
{
	return _ValueStack.back();
}

XE::WASMValue XE::WASMContext::Pop()
{
	auto val = Top();
	_ValueStack.pop_back();
	return val;
}

void XE::WASMContext::Pushed()
{
	_FrameStack.push_back( {} );
}

void XE::WASMContext::Poped()
{
	_FrameStack.pop_back();
}

XE::WASMFrame & XE::WASMContext::GetFrame()
{
	return _FrameStack.back();
}
