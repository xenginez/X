/*!
 * \file	AssetDatabase.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSETDATABASE_H__EA0B3B68_2ADC_4A3C_A01D_250E90E2006A
#define ASSETDATABASE_H__EA0B3B68_2ADC_4A3C_A01D_250E90E2006A

#include <QUrl>
#include <QUuid>
#include <QJsonDocument>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API AssetDatabase : public XE::EnableSharedFromThis< AssetDatabase >
{
private:
	struct Private;

public:
	AssetDatabase();

	~AssetDatabase();

public:
	bool Open( const QUrl & url );

public:
	bool Insert( const QUuid & uuid, const QJsonDocument & json );

	bool Remove( const QUuid & uuid );

	bool Update( const QUuid & uuid, const QJsonDocument & json );

	QJsonDocument Query( const QUuid & uuid );

private:
	Private * _p;
};

END_XS_NAMESPACE

#endif//ASSETDATABASE_H__EA0B3B68_2ADC_4A3C_A01D_250E90E2006A
