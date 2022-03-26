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

class XE_API MetaFunction : public MetaInfo
{
public:
	MetaFunction( const String & Name, MetaInfoPtr Result, ParameterType && Parameter, MetaModulePtr Module );

	~MetaFunction();

public:
	MetaInfoPtr GetResultType() const;

	const ParameterType & GetParameterType() const;

public:
	template< typename ... Types > Variant Invoke( Types &&... args ) const
	{
		InvokeStack stack;

		stack.Push( std::forward< Types >( args )... );

		return Invoke( &stack );
	}

public:
	virtual Variant Invoke( InvokeStack * params ) const = 0;

private:
	MetaInfoWPtr _Result;
	ParameterType _Parameter;
};

END_XE_NAMESPACE

#endif // METAFUNCTION_H__4B802A92_C70D_4C85_ABA7_AB7605E6FACE
