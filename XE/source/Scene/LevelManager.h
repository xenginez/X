/*!
 * \file   LevelManager.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/25
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef LEVELMANAGER_H__3E187969_1239_4F43_B1EC_1362D67CE0B5
#define LEVELMANAGER_H__3E187969_1239_4F43_B1EC_1362D67CE0B5

#include "Type.h"
#include "Core/Object.h"

BEG_XE_NAMESPACE

class XE_API Level : public XE::Object
{
	OBJECT( Level, XE::Object )

public:
	Level();

	~Level() override;

public:

private:
	XE::EntityManagerPtr _EntityManager;
	XE::ObjectManagerPtr _ObjectManager;
};

END_XE_NAMESPACE

#endif // LEVELMANAGER_H__3E187969_1239_4F43_B1EC_1362D67CE0B5
