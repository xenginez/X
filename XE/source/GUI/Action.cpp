#include "Action.h"

BEG_META( XE::ActionBase )
type->Property( "Callbacks", &XE::ActionBase::_Callbacks )->Attribute( XE::NonEditorAttribute() );
END_META()

XE::ActionBase::ActionBase()
{

}

XE::ActionBase::~ActionBase()
{

}

XE::Variant XE::ActionBase::Call( const XE::MetaClassCPtr & cls, XE::InvokeStack & args ) const
{
	XE::Variant result;
	for ( auto code : _Callbacks )
	{
		if ( auto method = cls->FindMethod( code ) )
		{
			result = method->Invoke( args );
		}
	}
	return result;
}
