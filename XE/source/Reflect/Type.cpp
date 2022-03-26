#include "Type.h"

#include "MetaInfo.h"
#include "Reflection.h"



XE::MetaException::MetaException( MetaInfoCPtr Meta, const String & Msg )
	: XE::RuntimeException( XE::Format( "" ) ), _Meta( Meta ), _Msg( Msg )
{

}

XE::MetaException::~MetaException()
{

}

char const* XE::MetaException::What() const
{
	return ( "MetaException: " + _Meta->GetFullName() + " " + _Msg ).c_str();
}

XE::MetaInfoCPtr XE::MetaException::GetMetaInfo() const
{
	return _Meta;
}
