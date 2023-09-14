/*!
 * \file	MetaFunction.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef METAFUNCTION_H__4B802A92_C70D_4C85_ABA7_AB7605E6FACE
#define METAFUNCTION_H__4B802A92_C70D_4C85_ABA7_AB7605E6FACE

#include "MetaInfo.h"
#include "InvokeStack.h"
#include "MetaTypeList.hpp"

BEG_XE_NAMESPACE

class XE_API MetaFunction : public XE::MetaInfo
{
public:
	MetaFunction( const XE::String & Name, XE::MetaInfoCPtr Result, XE::ParameterType && Parameter, XE::MetaModuleCPtr Module );

	~MetaFunction();

public:
	XE::MetaInfoCPtr GetResultType() const;

	const XE::ParameterType & GetParameterType() const;

public:
	template< typename ... Types > XE::Variant Invoke( Types &&... args ) const
	{
		XE::InvokeStack stack;

		stack.Push( std::forward< Types >( args )... );

		return Invoke( &stack );
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const = 0;

private:
	XE::MetaInfoCWPtr _Result;
	XE::ParameterType _Parameter;
};

END_XE_NAMESPACE

#endif // METAFUNCTION_H__4B802A92_C70D_4C85_ABA7_AB7605E6FACE
