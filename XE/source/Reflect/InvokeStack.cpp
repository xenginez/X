#include "InvokeStack.h"

XE::InvokeStack::InvokeStack()
{

}

XE::InvokeStack::InvokeStack( InvokeStack && val )
{
	std::swap( _Stack, val._Stack );
}

XE::InvokeStack::InvokeStack( const InvokeStack & val )
	:_Stack( val._Stack )
{

}

XE::InvokeStack::~InvokeStack()
{

}

XE::InvokeStack & XE::InvokeStack::operator=( InvokeStack && val )
{
	std::swap( _Stack, val._Stack );

	return *this;
}

XE::InvokeStack & XE::InvokeStack::operator=( const InvokeStack & val )
{
	_Stack = val._Stack;

	return *this;
}

void XE::InvokeStack::Push()
{

}

void XE::InvokeStack::Push( InvokeStack * val )
{
	_Stack.insert( _Stack.end(), val->_Stack.begin(), val->_Stack.end() );
}

void XE::InvokeStack::Push( InvokeStack & val )
{
	_Stack.insert( _Stack.end(), val._Stack.begin(), val._Stack.end() );
}

void XE::InvokeStack::Push( XE::Variant * val )
{
	_Stack.push_back( *val );
}

void XE::InvokeStack::Push( XE::Variant & val )
{
	_Stack.push_back( val );
}

void XE::InvokeStack::Push( const InvokeStack * val )
{
	_Stack.insert( _Stack.end(), val->_Stack.begin(), val->_Stack.end() );
}

void XE::InvokeStack::Push( const InvokeStack & val )
{
	_Stack.insert( _Stack.end(), val._Stack.begin(), val._Stack.end() );
}

void XE::InvokeStack::Push( const XE::Variant * val )
{
	_Stack.push_back( *val );
}

void XE::InvokeStack::Push( const XE::Variant & val )
{
	_Stack.push_back( val );
}

bool XE::InvokeStack::Empty() const
{
	return _Stack.size() == 0;
}

XE::uint64 XE::InvokeStack::GetSize() const
{
	return _Stack.size();
}

XE::ParameterType XE::InvokeStack::GetTypes() const
{
	XE::ParameterType types;

	for( const auto & it : _Stack )
	{
		types.push_back( it.GetType() );
	}

	return types;
}

void XE::InvokeStack::Clear()
{
	_Stack.clear();
}
