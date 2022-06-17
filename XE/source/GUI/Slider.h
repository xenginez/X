/*!
 * \file	Slider.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/17
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SLIDER_H__CC896864_ABE8_4725_BA77_8026369FA870
#define SLIDER_H__CC896864_ABE8_4725_BA77_8026369FA870

#include "Action.h"
#include "Widget.h"

BEG_XE_NAMESPACE

class XE_API Slider : public XE::Widget
{
	OBJECT( Slider, XE::Widget )

public:
	Slider();

	~Slider() override;

protected:
	void OnRender() override;

public:
	XE::int32 GetMin() const;

	void SetMin( XE::int32 val );

	XE::int32 GetMax() const;

	void SetMax( XE::int32 val );

	XE::int32 GetValue() const;

	void SetValue( XE::int32 val );

private:
	XE::int32 _Min = 0;
	XE::int32 _Max = 100;
	XE::int32 _Value = 0;
	XE::Action< void( XE::int32 ) > _OnValueChanged;
};

END_XE_NAMESPACE

#endif//SLIDER_H__CC896864_ABE8_4725_BA77_8026369FA870
