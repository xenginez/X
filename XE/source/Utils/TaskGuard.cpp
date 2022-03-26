#include "TaskGuard.h"

XE::TaskGuard::TaskGuard( const XE::Delegate< void() > & task )
	:_Task( task )
{

}

XE::TaskGuard::~TaskGuard()
{
	if( _Task )
	{
		_Task();
	}
}
