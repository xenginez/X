/*!
 * \file	Action.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ACTION_H__1D7FD514_7486_4A9C_83D7_917E66F1822A
#define ACTION_H__1D7FD514_7486_4A9C_83D7_917E66F1822A

#include "Controller.h"

BEG_XE_NAMESPACE

class XE_API ActionBase : public XE::Object
{
	OBJECT( ActionBase, XE::Object )

public:
	ActionBase();

	~ActionBase() override;

protected:
	XE::Variant Call( const XE::WidgetPtr & widget, const XE::ControllerPtr & controller, XE::InvokeStack & args ) const;

private:
	XE::Array< XE::uint64 > _Callbacks;
};

template< typename R, typename ... T > class Action< R( T ... ) > : public XE::ActionBase
{
public:
	Action() = default;

	Action( const ActionBase & val )
		: ActionBase( val )
	{

	}

	Action & operator=( const ActionBase & val )
	{
		ActionBase::operator=( val );

		return *this;
	}

	~Action() override = default;

public:
	R operator()( const XE::WidgetPtr & widget, const XE::ControllerPtr & controller, T ... args ) const
	{
		XE::InvokeStack stack;

		stack.Push( std::forward< T >( args )... );

		if constexpr ( std::is_void_v< R > )
		{
			ActionBase::Call( widget, controller, stack );
		}
		else
		{
			return ActionBase::Call( widget, controller, stack ).Value< R >();
		}
	}
};

END_XE_NAMESPACE

#endif//ACTION_H__1D7FD514_7486_4A9C_83D7_917E66F1822A
