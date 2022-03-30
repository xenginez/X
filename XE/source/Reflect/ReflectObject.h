/*!
 * \file	ReflectObject.h
 *
 * \author	ZhengYuanQing
 * \date	2019/09/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef REFLECTOBJECT_H__080637A5_D1CD_4182_81E3_57D5D2025E0D
#define REFLECTOBJECT_H__080637A5_D1CD_4182_81E3_57D5D2025E0D

#include "Variant.h"
#include "InvokeStack.h"
#include "MetaTypeList.hpp"

BEG_XE_NAMESPACE

class XE_API ReflectObject : public XE::EnableSharedFromThis< ReflectObject >
{
	template< typename T > friend struct MetaTypeCollector;

public:
	static MetaClassCPtr GetMetaClassStatic();

	virtual MetaClassCPtr GetMetaClass() const;

public:
	ReflectObject();

	virtual ~ReflectObject();

public:
	virtual XE::Variant Get( const XE::String & name );

	virtual void Set( const XE::String & name, const XE::Variant & val );

	virtual XE::Variant Invoke( const XE::String & name, XE::InvokeStack * params );

	template< typename ... Types > XE::Variant Invoke( const String & name, Types && ...args )
	{
		XE::InvokeStack stack;
		
		stack.Push( this, std::forward< Types >( args )... );

		return Invoke( name, &stack );
	}

public:
	virtual void Serialize( XE::OArchive & archive ) const;

	virtual void Deserialize( XE::IArchive & archive );

};

END_XE_NAMESPACE

#endif // REFLECTOBJECT_H__080637A5_D1CD_4182_81E3_57D5D2025E0D
