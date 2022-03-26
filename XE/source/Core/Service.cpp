#include "Service.h"

BEG_META( XE::Service )
END_META()

XE::Service::Service()
{

}

XE::Service::~Service()
{

}

XE::CoreFrameworkPtr XE::Service::GetFramework() const
{
	return _Framework;
}

void XE::Service::SetFramework( XE::CoreFrameworkPtr val )
{
	_Framework = val;
}
