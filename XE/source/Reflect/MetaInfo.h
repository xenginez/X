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
	MetaInfo( const XE::String & Name, XE::MetaInfoType Type, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module );

	virtual ~MetaInfo();

public:
	XE::MetaInfoType GetType() const;

	XE::uint64 GetHashCode() const;

	const XE::String & GetName() const;

	const XE::String & GetFullName() const;

	const XE::Array< XE::MetaAttributeCPtr > & GetAttributes() const;

public:
	XE::MetaInfoCPtr GetOwner() const;

	XE::MetaModuleCPtr GetModule() const;

public:
	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & val ) const;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( T::GetMetaClassStatic() ) );
	}

public:
	template< typename T > XE::MetaInfo * Attribute( const T & val )
	{
		_Attributes.push_back( XE::MakeShared< T >( val ) );

		return this;
	}

	template< typename T, typename ... ARGS > XE::MetaInfo * Attribute( const T & val, ARGS &&... args )
	{
		Attribute( val );

		Attribute( std::forward< ARGS >( args )... );

		return this;
	}

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
	XE::MetaInfoType _Type;
	XE::String _Name;
	XE::String _FullName;
	XE::uint64 _HashCode;
	XE::MetaInfoCWPtr _Owner;
	XE::MetaModuleCWPtr _Module;
	XE::Array< XE::MetaAttributeCPtr > _Attributes;
};

END_XE_NAMESPACE

#endif // __IMETA_H__7FB7B1B8_7091_4DBE_82E4_E95126647F93
