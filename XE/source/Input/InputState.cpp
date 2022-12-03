#include "InputState.h"

XE::InputState::InputState()
{

}

XE::InputState::~InputState()
{

}

XE::Variant XE::InputState::GetVariant( const XE::String & val ) const
{
	auto it = _Values.find( val );
	if ( it != _Values.end() )
	{
		return it->second;
	}
	return {};
}

bool XE::InputState::GetBool( const XE::String & val, bool def /*= false */ ) const
{
	auto it = _Values.find( val );
	if ( it != _Values.end() )
	{
		return it->second.Value< bool >();
	}
	return def;
}

XE::float32 XE::InputState::GetFloat( const XE::String & val, XE::float32 def /*= 0.0f */ ) const
{
	auto it = _Values.find( val );
	if ( it != _Values.end() )
	{
		return it->second.Value< XE::float32 >();
	}
	return def;
}

XE::BasicMemoryView< XE::CodePoint > XE::InputState::GetUnicodes() const
{
	return { _Unicodes.data(), _Unicodes.size() };
}

void XE::InputState::PutUnicode( XE::CodePoint val )
{
	_Unicodes.push_back( val );
}

void XE::InputState::PutVariant( const XE::String & name, const XE::Variant & val )
{
	_Values[name] = val;
}

void XE::InputState::Clear()
{
	_Values.clear();
	_Changes.clear();
	_Unicodes.clear();
}

void XE::InputState::ClearChanges()
{
	_Changes.clear();
}

void XE::InputState::Merge( const XE::InputState & val )
{
	_Unicodes.insert( _Unicodes.end(), val._Unicodes.begin(), val._Unicodes.end() );

	for ( const auto & value : val._Values )
	{
		auto it = _Values.find( value.first );
		if ( it != _Values.end() && it->second != value.second )
		{
			it->second = value.second;
			_Changes.insert( value );
		}
	}
}

const XE::Map< XE::String, XE::Variant > & XE::InputState::GetChanges() const
{
	return _Changes;
}
