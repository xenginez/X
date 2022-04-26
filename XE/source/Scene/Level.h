/*!
 * \file   Level.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/25
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227
#define LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Level : public XE::Object
{
	OBJECT( Level, XE::Object );

public:
	Level();

	~Level() override;

private:
	XE::LevelHandle _Handle;
};

END_XE_NAMESPACE

#endif // LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227
