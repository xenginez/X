/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__5DFAB569_85CF_410A_8A0D_5FCFF4D85232
#define DECLARE_H__5DFAB569_85CF_410A_8A0D_5FCFF4D85232

#include <set>
#include <map>
#include <list>
#include <deque>
#include <stack>
#include <queue>
#include <memory>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <functional>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>
#include <memory_resource>

#include "Config.h"

BEG_XE_NAMESPACE

using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;
using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;
using float32 = float;
using float64 = double;

class Bitmap;
class Exception;
class NonCopyable;

class CodePoint;
class AnsiEncode;
class WideEncode;
class Utf8Encode;
class StdStringConvert;
template < typename T > class Singleton;
template < typename Unit > class CodePointArrow;
template < typename Encode, typename Alloc > class BasicString;
template < typename Encode, typename Alloc > class BasicStringIterator;
template < typename StringIterator > class BasicStringReverseIterator;
template < typename FromEncode, typename ToEncode > class StringConvert;

template < typename T > using WeakPtr = std::weak_ptr< T >;
template < typename T > using SharedPtr = std::shared_ptr< T >;
template < typename T > using EnableSharedFromThis = std::enable_shared_from_this< T >;

template <class _Ty, XE::uint64 _Extent> class Span;

template < typename _Ty > class MultiArrayIterator;
template < typename _Ty > class MultiArrayConstIterator;
template < typename _Ty, typename ... _Size > class MultiArrayView;

template < typename ... Args > using Delegate = std::function< Args... >;

END_XE_NAMESPACE

#endif // DECLARE_H__5DFAB569_85CF_410A_8A0D_5FCFF4D85232
