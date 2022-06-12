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

void XE::ActionBase::Call( const XE::MetaClassCPtr & cls, XE::InvokeStack & args ) const
{
	for ( auto code : _Callbacks )
	{
		if ( auto method = cls->FindMethod( code ) )
		{
			method->Invoke( args );
		}
	}
}
