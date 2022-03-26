/*!
 * \file   MetaInfo.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETA_H__7FB7B1B8_7091_4DBE_82E4_E95126647F93
#define __IMETA_H__7FB7B1B8_7091_4DBE_82E4_E95126647F93

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API MetaInfo : public XE::EnableSharedFromThis< MetaInfo >
{
public:
	MetaInfo( const String & Name, MetaInfoType Type, MetaInfoPtr Owner, MetaModulePtr Module );

	virtual ~MetaInfo();

public:
	MetaInfoType GetType() const;

	XE::uint64 GetHashCode() const;

	const String & GetName() const;

	const String & GetFullName() const;

public:
	MetaInfoPtr GetOwner() const;

	MetaModulePtr GetModule() const;

public:
	template< typename T, typename ... Types > XE::SharedPtr< T > static NewMetaInfo( XE::uint64 code, Types &&... args )
	{
		auto pair = AllocMetaInfo( code, sizeof( T ) );

		if( !pair.first )
		{
			XE::SharedPtr< T > p( new ( pair.second ) T( args... ) );

			p->Register();

			return p;
		}

		return RP_CAST< T >( pair.second->shared_from_this() );
	}

private:
	static XE::Pair< bool, XE::MetaInfo * > AllocMetaInfo( XE::uint64 code, XE::uint64 size );

protected:
	void Register();

private:
	MetaInfoType _Type;
	String _Name;
	String _FullName;
	XE::uint64 _HashCode;
	MetaInfoWPtr _Owner;
	MetaModuleWPtr _Module;
};

END_XE_NAMESPACE

#endif // __IMETA_H__7FB7B1B8_7091_4DBE_82E4_E95126647F93
