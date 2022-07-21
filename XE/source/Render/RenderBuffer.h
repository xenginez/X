/*!
 * \file	RenderBuffer.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERBUFFER_H__0DF34CFB_C44E_4FE4_9822_8235D495092C
#define RENDERBUFFER_H__0DF34CFB_C44E_4FE4_9822_8235D495092C

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderBuffer : public XE::Object
{
	OBJECT( RenderBuffer, XE::Object )

public:
	RenderBuffer();

	~RenderBuffer() override;

};

END_XE_NAMESPACE

#endif//RENDERBUFFER_H__0DF34CFB_C44E_4FE4_9822_8235D495092C
