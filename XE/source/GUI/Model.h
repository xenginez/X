/*!
 * \file	Model.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MODEL_H__FDE62CE6_10CD_48D5_AD5B_1AEE1C44F1C0
#define MODEL_H__FDE62CE6_10CD_48D5_AD5B_1AEE1C44F1C0

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Model : public XE::Object
{
	OBJECT( Model, XE::Object )

public:
	Model();

	~Model() override;

public:
	virtual void Startup() = 0;

	virtual void Clearup() = 0;

public:
	XE::CanvasPtr GetCanvas() const;

private:
	XE::CanvasWPtr _Canvas;
};

END_XE_NAMESPACE

#endif//MODEL_H__FDE62CE6_10CD_48D5_AD5B_1AEE1C44F1C0
