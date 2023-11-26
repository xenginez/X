/*!
 * \file	AudioBuffer.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef AUDIOBUFFER_H__09ABB54B_87D3_49B2_87DA_A84502CE275F
#define AUDIOBUFFER_H__09ABB54B_87D3_49B2_87DA_A84502CE275F

#include "Reflect/Object.h"
#include "Utils/UUID.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API AudioBuffer : public XE::Object
{
	OBJECT( AudioBuffer, XE::Object )

public:
	AudioBuffer();

	~AudioBuffer() override;

public:
	XE::UUID GetUUID() const;

	void SetUUID( XE::UUID val );

	XE::uint32 GetBits() const;

	void SetBits( XE::uint32 val );

	XE::uint32 GetChannels() const;

	void SetChannels( XE::uint32 val );

	XE::uint32 GetFrequency() const;

	void SetFrequency( XE::uint32 val );

private:
	XE::UUID _UUID;
	XE::uint32 _Bits = 8;
	XE::uint32 _Channels = 1;
	XE::uint32 _Frequency = 16000;
};

END_XE_NAMESPACE

#endif//AUDIOBUFFER_H__09ABB54B_87D3_49B2_87DA_A84502CE275F
