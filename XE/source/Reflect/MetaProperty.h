/*!
 * \file   MetaProperty.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621
#define __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621

#include "MetaInfo.h"
#include "MetaAttribute.h"

BEG_XE_NAMESPACE

class XE_API MetaProperty : public MetaInfo
{
public:
	MetaProperty( const String & Name, bool IsStatic, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypePtr Value, MetaClassPtr Owner, MetaModulePtr Module );

	~MetaProperty();

public:
	bool IsConst() const;

	bool IsStatic() const;

	bool IsObject() const;

	bool IsPointer() const;

	bool IsSharedPtr() const;

	bool IsReference() const;

public:
	MetaTypePtr GetValueType() const;

	const XE::Array< XE::MetaAttributeCPtr > & GetAttributes() const;

public:
	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & val ) const;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( ClassID< T >::Get() ) );
	}

public:
	XE::Variant Get( const XE::Variant & obj ) const;

	void Set( const XE::Variant & obj, const XE::Variant & val ) const;

protected:
	virtual XE::Variant Getter( const XE::Variant & obj ) const = 0;

	virtual void Setter( const XE::Variant & obj, const XE::Variant & val ) const = 0;

private:
	bool _IsConst;
	bool _IsStatic;
	bool _IsObject;
	bool _IsPointer;
	bool _IsSharedPtr;
	bool _IsReference;
	XE::MetaTypeWPtr _Value;

protected:
	XE::Array< XE::MetaAttributeCPtr > _Attributes;
};

END_XE_NAMESPACE

#endif // __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621
