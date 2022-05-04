/*!
 * \file   World.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D
#define WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API World : public XE::Object
{
	OBJECT( World, XE::Object )

public:
	World();

	~World() override;

public:
	void Prepare();

	void Startup();

	void Update( XE::float32 dt );

	void Clearup();

public:
	bool IsPrepare() const;

private:
	bool _IsPrepare = false;
	XE::Array< XE::LevelPtr > _Levels;
};

END_XE_NAMESPACE

#endif // WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D
