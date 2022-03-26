#include "Disposable.h"

XE::Disposable::Disposable( Disposable && val )
{
	std::swap( _Callback, val._Callback );
}

XE::Disposable::Disposable( const CallbackType & val )
	: _Callback( val )
{}

XE::Disposable::~Disposable()
{
	Dispose();
}

XE::Disposable & XE::Disposable::operator=( Disposable && val )
{
	std::swap( _Callback, val._Callback );

	return *this;
}

void XE::Disposable::Detach()
{
	_Callback = nullptr;
}

void XE::Disposable::Dispose()
{
	if( _Callback )
	{
		_Callback();
		_Callback = nullptr;
	}
}
