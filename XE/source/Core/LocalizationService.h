/*!
 * \file   LocalizationService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/05
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __LOCALIZATIONSERVICE_H__547E0953_14F0_436F_A09E_141D3E4DAA47
#define __LOCALIZATIONSERVICE_H__547E0953_14F0_436F_A09E_141D3E4DAA47

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API LocalizationService : public XE::Service
{
	OBJECT( LocalizationService, Service )

private:
	struct Private;

public:
	LocalizationService();

	~LocalizationService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	virtual XE::Language GetCurrentLanguage() const;

	virtual void SetCurrentLanguage( XE::Language val );

public:
	virtual const String & LocalizedString( const XE::String & key ) const;

private:
	void LoadLocalized();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __LOCALIZATIONSERVICE_H__547E0953_14F0_436F_A09E_141D3E4DAA47
