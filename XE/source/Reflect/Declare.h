/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__B2A36370_706B_42B8_B679_1A18124E5DD3
#define DECLARE_H__B2A36370_706B_42B8_B679_1A18124E5DD3

#include "Memory/Declare.h"

BEG_XE_NAMESPACE

class Variant;

class OArchive;
class IArchive;

class MetaAttribute;
class RangeAttribute;
class NonCloneAttribute;
class NonInspectorAttribute;
class NonSerializeAttribute;

class MetaInfo;
class MetaType;
class MetaEnum;
class MetaClass;
class MetaModule;
class MetaMethod;
class MetaProperty;
class MetaFunction;
class MetaVariable;

class MetaEnumerator;

class Object;
class Reflection;

class InvokeStack;
class TemplateType;
class ParameterType;

template< typename T > struct Cloneable;
template< typename T > struct Serializable;

template< typename T > class ArchiveNameValue;

template< typename T > struct VariantCreate;
template< typename T > struct VariantCast;

template< typename ... Types > struct TypeList;


template< typename T > class CXXMetaEnum;
template< typename T > class CXXMetaClass;
template< typename T, typename ... Types > class CXXTplMetaClass;
template< typename T > class CXXMetaFundamental;
template< typename T  > class CXXMetaMethod;
template< typename ... T > class CXXMetaProperty;
template< typename T > class CXXMetaFunction;
template< typename T > class CXXMetaVariable;
template< typename T > class CXXMetaEnumerator;

END_XE_NAMESPACE

template< typename T > struct TypeID;
template< typename T > struct EnumID;
template< typename T > struct ClassID;

#endif // DECLARE_H__B2A36370_706B_42B8_B679_1A18124E5DD3
