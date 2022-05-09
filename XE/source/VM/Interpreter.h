/*!
 * \file   Interpreter.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/07
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef INTERPRETER_H__91821F2A_A9C7_4585_8D94_D1BD08B348FD
#define INTERPRETER_H__91821F2A_A9C7_4585_8D94_D1BD08B348FD

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Interpreter : public std::enable_shared_from_this< Interpreter >
{
public:
	Interpreter();

	~Interpreter();

public:
	XE::int32 Invoke( const XE::ModulePtr & mod, XE::uint32 func );

public:
	void Push( const XE::Value & val );

	XE::Value Pop();

private:
	void Pushed( const XE::Frame & frame );

	XE::Frame Poped();

private:
	XE::Deque< XE::Frame > _FrameStack;
	XE::Deque< XE::Value > _ValueStack;
};

END_XE_NAMESPACE

#endif // INTERPRETER_H__91821F2A_A9C7_4585_8D94_D1BD08B348FD
