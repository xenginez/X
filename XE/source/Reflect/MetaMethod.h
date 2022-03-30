/*!
 * \file   MetaMethod.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETAMETHOD_H__154D0025_BEE1_4DFB_AC1F_4739AFB4B34F
#define __IMETAMETHOD_H__154D0025_BEE1_4DFB_AC1F_4739AFB4B34F

#include "MetaType.h"
#include "InvokeStack.h"
#include "MetaTypeList.hpp"

BEG_XE_NAMESPACE

class XE_API MetaMethod : public MetaInfo
{
public:
	MetaMethod( const String & Name, bool IsStatic, bool IsConst, MetaTypeCPtr Result, ParameterType && Parameter, MetaClassCPtr Owner, MetaModuleCPtr Module );

	~MetaMethod();

public:
	bool IsConst() const;

	bool IsStatic() const;

public:
	XE::MetaTypeCPtr GetResultType() const;

	const XE::ParameterType & GetParameterType() const;

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const = 0;

	template< typename ... Types > XE::Variant Invoke( Types &&... args ) const
	{
		XE::InvokeStack stack;

		stack.Push( std::forward< Types >( args )... );

		return Invoke( &stack );
	}

private:
	bool _IsConst;
	bool _IsStatic;
	XE::MetaTypeCWPtr _Result;
	XE::ParameterType _Parameter;
};

END_XE_NAMESPACE

#endif // __IMETAMETHOD_H__154D0025_BEE1_4DFB_AC1F_4739AFB4B34F
