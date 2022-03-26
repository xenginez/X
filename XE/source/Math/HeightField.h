/*!
 * \file	HeightField.h
 *
 * \author	ZhengYuanQing
 * \date	2021/12/04
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef HEIGHTFIELD_H__25DABC05_3096_40E7_A247_1F10298A3485
#define HEIGHTFIELD_H__25DABC05_3096_40E7_A247_1F10298A3485

#include "float16.h"

BEG_XE_NAMESPACE

class XE_API HeightField
{
public:
	XE::float16 min;
	XE::float16 max;
	XE::Array< XE::float16 > data;

};
DECL_XE_CLASS( HeightField );

END_XE_NAMESPACE

#endif//HEIGHTFIELD_H__25DABC05_3096_40E7_A247_1F10298A3485
