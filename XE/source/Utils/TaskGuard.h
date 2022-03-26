/*!
 * \file	TaskGuard.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/08/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TASKGUARD_HPP__7A59D6ED_A20B_4557_AFB5_A1C03E3A6555
#define TASKGUARD_HPP__7A59D6ED_A20B_4557_AFB5_A1C03E3A6555

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API TaskGuard : public XE::NonCopyable
{
public:
	TaskGuard( const XE::Delegate< void() > & task );

	~TaskGuard();

private:
	XE::Delegate< void() > _Task;
};

END_XE_NAMESPACE

#endif // TASKGUARD_HPP__7A59D6ED_A20B_4557_AFB5_A1C03E3A6555
