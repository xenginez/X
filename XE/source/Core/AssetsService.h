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

public:
	using LoadFinishCallback = XE::Delegate< void( XE::MemoryView ) >;

	using LoadObjectFinishCallback = XE::Delegate< void( XE::ObjectPtr ) >;

	using LoadStreamFinishCallback = XE::Delegate< void( XE::MemoryView ) >;

private:
	struct Private;

public:
	AssetsService();

	~AssetsService() override;

public:
	void Prepare()override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::Array< XE::UUID > GetAssetsFromType( const XE::String & type ) const;

public:
	virtual XE::MemoryView Load( const XE::UUID & uuid );

	virtual XE::MemoryView Load( const XE::UUID & uuid, const XE::String & path );

	virtual void AsyncLoad( const XE::UUID & uuid, const LoadFinishCallback & callback );

	virtual void AsyncLoad( const XE::UUID & uuid, const XE::String & path, const LoadFinishCallback & callback );

public:
	virtual XE::ObjectPtr LoadObject( const XE::UUID & uuid );

	virtual XE::ObjectPtr LoadObject( const XE::UUID & uuid, const XE::String & path );

	virtual void AsyncLoadObject( const XE::UUID & uuid, const LoadObjectFinishCallback & callback );

	virtual void AsyncLoadObject( const XE::UUID & uuid, const XE::String & path, const LoadObjectFinishCallback & callback );

public:
	virtual bool LoadStream( const XE::UUID & uuid, std::ostream & stream, XE::uint64 offset = 0, XE::uint64 size = std::numeric_limits< XE::uint64 >::max() );

	virtual bool LoadStream( const XE::UUID & uuid, const XE::String & path, std::ostream & stream, XE::uint64 offset = 0, XE::uint64 size = std::numeric_limits< XE::uint64 >::max() );

	virtual void AsyncLoadStream( const XE::UUID & uuid, XE::uint64 offset, XE::uint64 size, const LoadStreamFinishCallback & callback );

	virtual void AsyncLoadStream( const XE::UUID & uuid, const XE::String & path, XE::uint64 offset, XE::uint64 size, const LoadStreamFinishCallback & callback );

public:
	template< typename T > T LoadObjectT( const XE::UUID & uuid, const XE::String & path )
	{
		return DP_CAST< T::element_type >( LoadObject( uuid, path ) );
	}

	template< typename T > void AsyncLoadObjectT( const XE::UUID & uuid, const XE::String & path, const XE::Delegate< void( T ) > & callback )
	{
		AsyncLoadObject( uuid, path, [callback]( XE::ObjectPtr obj )
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
