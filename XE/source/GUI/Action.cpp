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

XE::Variant XE::ActionBase::Call( const XE::WidgetPtr & widget, const XE::ControllerPtr & controller, XE::InvokeStack & args ) const
{
	XE::Variant result;
	if ( auto cls = controller->GetMetaClass() )
	{
		controller->SetWidget( widget );

		for ( auto code : _Callbacks )
		{
			if ( auto method = cls->FindMethod( code ) )
			{
				result = method->Invoke( controller, args );
			}
		}

		controller->SetWidget( nullptr );
	}
	return result;
}
