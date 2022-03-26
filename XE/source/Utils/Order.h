/*!
 * \file   Order.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __ORDER_H__F15EE138_6173_4905_9684_08685B5F706A
#define __ORDER_H__F15EE138_6173_4905_9684_08685B5F706A

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Order : public XE::NonCopyable
{
private:
	struct Private;

public:
	static const XE::uint64 Invalid = 0;

private:
	Order();

	~Order();

	static Order * Instance();

public:
	template< typename T > static XE::uint64 RegisterOrder( const String& name, const String& desc, const MetaInfoPtr & parameter )
	{
		return RegisterOrder( T::group_value, name, desc, parameter );
	}

	template< typename T > static XE::uint64 FindOrderID( const String& name )
	{
		return FindOrderID( T::group_value, name );
	}

	template< typename T > static void VisitOrder( XE::Delegate< void( XE::uint64, String, String, const MetaInfoPtr & ) > val )
	{
		VisitOrder( T::group_value, val );
	}

public:
	static String FindOrderName( XE::uint64 id );

	static String FindOrderDesc( XE::uint64 id );

	static MetaInfoPtr FindOrderPatameter( XE::uint64 id );

public:
	static XE::uint64 RegisterOrder( XE::uint8 group, const String& name, const String& desc, const MetaInfoPtr & parameter );

	static XE::uint64 FindOrderID( XE::uint8 group, const String& name );

	static void VisitOrder( XE::uint8 group, XE::Delegate< void( XE::uint64, String, String, const MetaInfoPtr & ) > val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#define DECL_GROUP( _DLL_EXPORT, INDEX, NAME ) \
template< > class _DLL_EXPORT XE::OrderGroup< INDEX > \
{ \
public: \
    static constexpr XE::uint8 group_value = INDEX; \
}; \
namespace XE{ using NAME##Group = XE::OrderGroup< INDEX >; }

#endif // __ORDER_H__F15EE138_6173_4905_9684_08685B5F706A
