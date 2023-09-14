#include "MetaType.h"

XE::MetaType::MetaType( const XE::String & Name, XE::MetaInfoType Type, XE::uint64 Size, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module )
	: XE::MetaInfo( Name, Type, Owner, Module ), _Size( Size )
{

}

XE::MetaType::~MetaType()
{

}

XE::uint64 XE::MetaType::GetSize() const
{
	return _Size;
}
