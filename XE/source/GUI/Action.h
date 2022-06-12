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

#include "Model.h"

BEG_XE_NAMESPACE

class XE_API ActionBase : public XE::Object
{
	OBJECT( ActionBase, XE::Object )

public:
	ActionBase();

	~ActionBase() override;

protected:
	void Call( const XE::MetaClassCPtr & cls, XE::InvokeStack & args ) const;

private:
	XE::Array< XE::uint64 > _Callbacks;
};

template< typename ... T > class Action : public XE::ActionBase
{
public:
	Action() = default;

	~Action() override = default;

public:
	void Call( const XE::ModelPtr & model, T &&...args ) const
	{
		XE::InvokeStack stack;
		stack.Push( model );
		stack.Push( args... );

		ActionBase::Call( model->GetMetaClass(), stack );
	}
};
IMPLEMENT_XE_TEMPLATE_CLASS( Action );

END_XE_NAMESPACE

#endif//ACTION_H__1D7FD514_7486_4A9C_83D7_917E66F1822A
