#include "ASTInstance.h"

XE::ASTInstance::ASTInstance()
{

}

XE::ASTInstance::~ASTInstance()
{

}

XE::uint64 XE::ASTInstance::GetStackPoint() const
{
	return _Stack.size();
}

XE::uint64 XE::ASTInstance::PushStack( const XE::Variant & val )
{
	_Stack.push_back( val );
}

XE::Variant XE::ASTInstance::PopStack()
{
	auto val = _Stack.back();
	_Stack.pop_back();
	return val;
}

XE::Variant & XE::ASTInstance::TopStack() const
{
	return _Stack.back();
}

XE::Variant & XE::ASTInstance::GetStack( XE::uint64 sp )
{
	return _Stack[sp];
}

XE::uint64 XE::ASTInstance::ResetStack( XE::uint64 sp )
{
	_Stack.resize( sp );

	return sp;
}
