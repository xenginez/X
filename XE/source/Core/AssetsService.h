/*!
 * \file   AssetsService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/13
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __ASSETSSERVICE_H__68A7444E_AF99_4A9A_804F_B524B90E80EC
#define __ASSETSSERVICE_H__68A7444E_AF99_4A9A_804F_B524B90E80EC

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API AssetsService : public XE::Service
{
	OBJECT( AssetsService, Service )

private:
	struct Private;

public:
	using LoadFinishCallback = XE::Delegate< void( XE::MemoryView ) >;
	using LoadObjectFinishCallback = XE::Delegate< void( XE::ObjectPtr ) >;
	
public:
	AssetsService();

	~AssetsService() override;

public:
	void Prepare()override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	virtual XE::MemoryView Load( const XE::UUID & uuid );

	virtual XE::MemoryView Load( const XE::String & name );

	virtual void AsyncLoad( const XE::UUID & uuid, const LoadFinishCallback & callback );

	virtual void AsyncLoad( const XE::String & name, const LoadFinishCallback & callback );

public:
	virtual XE::ObjectPtr LoadObject( const XE::UUID & uuid );

	virtual XE::ObjectPtr LoadObject( const XE::String & name );

	virtual void AsyncLoadObject( const XE::UUID & uuid, const LoadObjectFinishCallback & callback );

	virtual void AsyncLoadObject( const XE::String & name, const LoadObjectFinishCallback & callback );

public:
	template< typename T > T LoadObjectT( const XE::UUID & uuid )
	{
		return DP_CAST< T::element_type >( LoadObject( uuid ) );
	}

	template< typename T > T LoadObjectT( const XE::String & name )
	{
		return DP_CAST< T::element_type >( LoadObject( name ) );
	}

	template< typename T > void AsyncLoadObjectT( const XE::UUID & uuid, const XE::Delegate< void( T ) > & callback )
	{
		AsyncLoadObject( uuid, [callback]( XE::ObjectPtr obj )
		{
			if ( callback )
			{
				callback( DP_CAST< T::element_type >( obj ) );
			}
		} );
	}

	template< typename T > void AsyncLoadObjectT( const XE::String & name, const XE::Delegate< void( T ) > & callback )
	{
		AsyncLoadObject( name, [callback]( XE::ObjectPtr obj )
						 {
							 if( callback )
							 {
								 callback( DP_CAST< T::element_type >( obj ) );
							 }
						 } );
	}

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __ASSETSSERVICE_H__68A7444E_AF99_4A9A_804F_B524B90E80EC
