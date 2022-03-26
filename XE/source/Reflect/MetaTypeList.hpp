/*!
 * \file   ParameterType.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __PARAMETERTYPE_HPP__DD6460A4_04AB_4F79_8A04_4C4CF356BDC4
#define __PARAMETERTYPE_HPP__DD6460A4_04AB_4F79_8A04_4C4CF356BDC4

#include "MetaType.h"

BEG_XE_NAMESPACE

template< typename T, typename ... Types > struct TypeList< T, Types... >
{
	static void Make( XE::Array< XE::MetaTypePtr > * args )
	{
		args->push_back( TypeID< typename TypeTraits< T >::raw_t >::Get() );

		TypeList< Types... >::Make( args );
	}
};

template< typename T > struct TypeList< T >
{
	static void Make( XE::Array< XE::MetaTypePtr > * args )
	{
		args->push_back( TypeID< typename TypeTraits< T >::raw_t >::Get() );
	}
};

template< > struct TypeList< >
{
	static void Make( XE::Array< XE::MetaTypePtr > * args )
	{
	}
};


class XE_API TemplateType : public XE::Array< XE::MetaTypePtr >
{
public:
	using XE::Array< XE::MetaTypePtr >::operator[];
};
class XE_API ParameterType : public XE::Array< XE::MetaTypePtr >
{
public:
	using XE::Array< XE::MetaTypePtr >::operator[];
};


template< typename ... Types >TemplateType MakeTemplateType()
{
	TemplateType ret;
	XE::TypeList< Types... >::Make( &ret );
	return ret;
}

template< typename ... Types >ParameterType MakeParameterType()
{
	ParameterType ret;
	XE::TypeList< Types... >::Make( &ret );
	return ret;
}


XE_INLINE XE::String ToString( const TemplateType & _Val )
{
	XE::String ret = "< ";

	for( int i = 0; i < _Val.size(); ++i )
	{
		ret += _Val[i]->GetFullName();
		if( i != _Val.size() - 1 )
		{
			ret += ", ";
		}
	}

	return ret + " >";
}

XE_INLINE XE::String ToString( const ParameterType & _Val )
{
	XE::String ret = "( ";

	for( int i = 0; i < _Val.size(); ++i )
	{
		ret += _Val[i]->GetFullName();
		if( i != _Val.size() - 1 )
		{
			ret += ", ";
		}
	}

	return ret + " )";
}

END_XE_NAMESPACE

#endif // __PARAMETERTYPE_HPP__DD6460A4_04AB_4F79_8A04_4C4CF356BDC4
