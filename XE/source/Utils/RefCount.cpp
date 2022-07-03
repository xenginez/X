#include "RefCount.h"

void XE::RefCount::Reset()
{
	_Count = 0;
}

XE::uint64 XE::RefCount::Inc()
{
	return ++_Count;
}

XE::uint64 XE::RefCount::Dec()
{
	auto count = --_Count;
	if ( count == 0 && _Deleter )
	{
		_Deleter( this );
	}
	return count;
}

XE::uint64 XE::RefCount::Count() const
{
	return _Count.load();
}
