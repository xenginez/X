/*!
 * \file	MetaVariable.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA
#define METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA

#include "MetaInfo.h"

BEG_XE_NAMESPACE

class XE_API MetaVariable : public MetaInfo
{
public:
	MetaVariable( const String & Name, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypeCPtr Value, MetaModuleCPtr Module );

	~MetaVariable();

public:
	bool IsConst() const;

	bool IsObject() const;

	bool IsPointer() const;

	bool IsSharedPtr() const;

	bool IsReference() const;

public:
	MetaTypeCPtr GetValueType() const;

	const XE::Array< XE::MetaAttributeCPtr > & GetAttributes() const;

public:
	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & val ) const;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( ClassID< T >::Get() ) );
	}

public:
	template< typename T > MetaVariable * Attribute( const T & val )
	{
		static_assert( std::is_base_of_v< XE::MetaAttribute, XE::TypeTraits< T >::raw_t >, "does not belong to meta attribute" );

		_Attributes.push_back( XE::MakeShared< T >( val ) );

		return this;
	}

	template< typename T, typename ... ARGS > MetaVariable * Attribute( const T & val, ARGS &&... args )
	{
		Attribute( val );

		Attribute( std::forward< ARGS >( args )... );

		return this;
	}
public:
	XE::Variant Get() const;

	void Set(const XE::Variant & val ) const;

protected:
	virtual XE::Variant Getter() const = 0;

	virtual void Setter(const XE::Variant & val ) const = 0;

private:
	bool _IsConst;
	bool _IsObject;
	bool _IsPointer;
	bool _IsSharedPtr;
	bool _IsReference;
	XE::MetaTypeCWPtr _Value;
	XE::Array< XE::MetaAttributeCPtr > _Attributes;
};

END_XE_NAMESPACE

#endif // METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA
