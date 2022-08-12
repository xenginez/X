/*!
 * \file   Config.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CONFIG_H__851B77A2_E78D_4E14_91A6_D6317250A486
#define __CONFIG_H__851B77A2_E78D_4E14_91A6_D6317250A486

/////////////////////////////////////////////////////////////////////////////////////////
/// CPU
#define CPU_AMD64 1
#define CPU_ARM64 2
#define CPU_RISCV 3

#if defined(_AMD64_) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__)
#	define CPU CPU_AMD64
#elif defined(_ARM64_) || defined(__aarch64__) || defined(_M_ARM64)       
#	define CPU CPU_ARM64
#elif defined(_RISCV_) ||defined(__riscv) || defined(__riscv__) || defined(RISCVEL)
#	define CPU CPU_RISCV
#else
#   error "unknown cpu"
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// PLATFORM OS

#define OS_MAC			1 << 1
#define OS_IOS			1 << 2
#define OS_XBOX			1 << 3
#define OS_LINUX		1 << 4
#define OS_WINDOWS		1 << 5
#define OS_ANDROID		1 << 6
#define OS_NINTENDO		1 << 7
#define OS_HTML5		1 << 8
#define OS_PLAY_STATION	1 << 9

#ifdef _WIN32
#		define PLATFORM_OS OS_WINDOWS
#elif __EMSCRIPTEN__
#		define PLATFORM_OS OS_HTML5
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
#		define PLATFORM_OS OS_IOS
#elif TARGET_OS_IPHONE
#		define PLATFORM_OS OS_IOS
#elif TARGET_OS_MAC
#		define PLATFORM_OS OS_MAC
#endif
#elif __ANDROID__
#		define PLATFORM_OS OS_ANDROID
#elif __linux__
#		define PLATFORM_OS OS_LINUX
#else
#   error "unknown platform"
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// GRAPHICS
#define GRAPHICS_D3D11     1 << 1
#define GRAPHICS_D3D12     1 << 2
#define GRAPHICS_METAL     1 << 3
#define GRAPHICS_VULKAN    1 << 4
#define GRAPHICS_WEBGPU    1 << 5
#define GRAPHICS_OPENGL    1 << 6
#define GRAPHICS_OPENGLES  1 << 7

#if   ( PLATFORM_OS & OS_WINDOWS )
#define GRAPHICS_API ( GRAPHICS_D3D11 | GRAPHICS_D3D12 | GRAPHICS_VULKAN | GRAPHICS_OPENGL | GRAPHICS_OPENGLES )
#elif ( PLATFORM_OS & OS_XBOX )
#define GRAPHICS_API ( GRAPHICS_D3D12 )
#elif ( PLATFORM_OS & OS_MAC )
#define GRAPHICS_API ( GRAPHICS_METAL | GRAPHICS_VULKAN | GRAPHICS_OPENGL | GRAPHICS_OPENGLES )
#elif ( PLATFORM_OS & OS_IOS )
#define GRAPHICS_API ( GRAPHICS_METAL | GRAPHICS_OPENGLES )
#elif ( PLATFORM_OS & OS_LINUX )
#define GRAPHICS_API ( GRAPHICS_VULKAN | GRAPHICS_OPENGL | GRAPHICS_OPENGLES )
#elif ( PLATFORM_OS & OS_ANDROID )
#define GRAPHICS_API ( GRAPHICS_VULKAN | GRAPHICS_OPENGLES )
#elif ( PLATFORM_OS & OS_HTML5 )
#define GRAPHICS_API ( GRAPHICS_WEBGPU )
#else
#define GRAPHICS_API ( 0 )
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// COMPILER

#define COMPILER_GCC 1
#define COMPILER_MSVC 2
#define COMPILER_EMCC 3
#define COMPILER_CLANG 4

#if defined( __GNUC__ )
#   define COMPILER COMPILER_GCC
#elif defined( _MSC_VER )
#   define COMPILER COMPILER_MSVC
#elif defined( __EMSCRIPTEN__ )
#   define COMPILER COMPILER_EMCC
#elif defined( __clang__ )
#   define COMPILER COMPILER_CLANG
#else
#   error "unknown compiler !"
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// CPU ARCH

#define ARCH_32 1
#define ARCH_64 2

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) || defined(__64BIT__) || defined(_ARM64_) || defined(__mips64) || defined(__powerpc64__) || defined(__ppc64__) || defined(__LP64__)
#   define ARCH_TYPE ARCH_64
#else
#   define ARCH_TYPE ARCH_32
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// SIMD 
#define SIMD_SSE 1
#define SIMD_NEON 2

#if (defined (_M_IX86) || defined (_M_X64)) && !defined(_CHPE_ONLY_)
#	define SIMD_TYPE SIMD_SSE
#elif defined (_M_ARM) || defined (_M_ARM64) || defined(_M_HYBRID_X86_ARM64)
#	define SIMD_TYPE SIMD_NEON
#else
#	define SIMD_TYPE SIMD_SSE
//#   error "unknown SIMD !"
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// INLINE

#if COMPILER == COMPILER_MSVC
#	define XE_INLINE __forceinline
#elif COMPILER == COMPILER_GCC || COMPILER == COMPILER_CLANG
#	define XE_INLINE __inline__
#else
#	define XE_INLINE inline
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// DLL EXPORT

#if COMPILER == COMPILER_MSVC
#   define DLL_IMPORT __declspec( dllimport )
#   define DLL_EXPORT __declspec( dllexport )
#   define DLL_VAR_WEAK __declspec( selectany )
#elif COMPILER == COMPILER_EMCC
#   define DLL_IMPORT 
#   define DLL_EXPORT EMSCRIPTEN_KEEPALIVE
#   define DLL_VAR_WEAK EMSCRIPTEN_KEEPALIVE
#elif COMPILER == COMPILER_GCC || COMPILER == COMPILER_CLANG
#   define DLL_IMPORT __attribute__ ((visibility ("default")))
#   define DLL_EXPORT __attribute__ ((visibility ("default")))
#   define DLL_VAR_WEAK __attribute__((weak))
#endif

#if PLATFORM_OS & OS_WINDOWS
#	define DLL_EXT_NAME ".dll"
#elif PLATFORM_OS & OS_XBOX
#	define DLL_EXT_NAME ".dll"
#elif PLATFORM_OS & OS_ANDROID
#	define DLL_EXT_NAME ".so"
#elif PLATFORM_OS & OS_LINUX
#	define DLL_EXT_NAME ".so"
#elif PLATFORM_OS & OS_IOS
#	define DLL_EXT_NAME ".dylib"
#elif PLATFORM_OS & OS_MAC
#	define DLL_EXT_NAME ".dylib"
#elif PLATFORM_OS & OS_PLAY_STATION
#	define DLL_EXT_NAME ".so"
#elif PLATFORM_OS & OS_HTML5
#	define DLL_EXT_NAME ".wasm"
#else
#   error "unknown PLATFORM_OS!"
#endif

#ifdef XE_EXPORT
#	ifdef X_USE_SHARED
#		define XE_API DLL_EXPORT
#	else
#		define XE_API 
#	endif // XE_SHARED_LIBS
#else
#	ifdef XE_SHARED_LIBS
#		define XE_API DLL_IMPORT
#	else
#		define XE_API 
#	endif // XE_SHARED_LIBS
#endif // XE_EXPORT


/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// ALIGNED

#define ALIGNED(VALUE, ALIGN) ((XE::uint64(VALUE) + (XE::uint64(ALIGN) - 1)) & ~(XE::uint64(ALIGN) - 1))
#define ALIGNED4(VALUE) ALIGNED(VALUE, 4)
#define ALIGNED8(VALUE) ALIGNED(VALUE, 8)
#define ALIGNED16(VALUE) ALIGNED(VALUE, 16)
#define ALIGNED32(VALUE) ALIGNED(VALUE, 32)
#define ALIGNED64(VALUE) ALIGNED(VALUE, 64)
#define ALIGNED128(VALUE) ALIGNED(VALUE, 128)
#define ALIGNED256(VALUE) ALIGNED(VALUE, 256)

#define IS_ALIGNED(VALUE, ALIGN) ((XE::uint64(VALUE) & XE::uint64(ALIGN)) == 0)
#define IS_ALIGNED4(VALUE) IS_ALIGNED(VALUE, 4)
#define IS_ALIGNED8(VALUE) IS_ALIGNED(VALUE, 8)
#define IS_ALIGNED16(VALUE) IS_ALIGNED(VALUE, 16)
#define IS_ALIGNED32(VALUE) IS_ALIGNED(VALUE, 32)
#define IS_ALIGNED64(VALUE) IS_ALIGNED(VALUE, 64)
#define IS_ALIGNED128(VALUE) IS_ALIGNED(VALUE, 128)
#define IS_ALIGNED256(VALUE) IS_ALIGNED(VALUE, 256)

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// DEBUG

#if defined(DEBUG) || defined(_DEBUG)
#	define XE_DEBUG 1
#endif // DEBUG

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// ASSERT

#ifndef XE_DEBUG
#	define XE_ASSERT(x) { if(!(x)){ (void)0; } }
#else
#	if PLATFORM_OS & OS_WINDOWS
#		define XE_ASSERT(x) { if(!(x)) __debugbreak(); }
#	else
#		define XE_ASSERT(x) { if(!(x)) raise(SIGTRAP); }
#	endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// MEMORY SIZE

#define ONE_BYTE ( unsigned long long( 1 ) )
#define ONE_KBYTE ( ONE_BYTE * 1024 )
#define ONE_MBYTE ( ONE_KBYTE * 1024 )
#define ONE_GBYTE ( ONE_MBYTE * 1024 )
#define BYTE( COUNT )  ( ONE_BYTE * ( COUNT ) )
#define KBYTE( COUNT ) ( ONE_KBYTE * ( COUNT ) )
#define MBYTE( COUNT ) ( ONE_MBYTE * ( COUNT ) )
#define GBYTE( COUNT ) ( ONE_GBYTE * ( COUNT ) )

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// MACRO ARGS COUNT

#define MACRO_TO_STRING_(x) #x
#define MACRO_TO_STRING(x) MACRO_TO_STRING_(x)

#define MACRO_CAT(x, y)     x##y
#define MACRO_GLUE(x, y)    MACRO_CAT(x, y)

#define MACRO_EXP_(exp) exp
#define MACRO_ARGS_FILTER(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_N, ...) _N
#define MACRO_ARGS_CONTER(...)  MACRO_EXP_(MACRO_ARGS_FILTER(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// NAMESPACE

#define BEG_XE_NAMESPACE namespace XE {
#define END_XE_NAMESPACE };

/////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////
/// IGNORED WARNING

#if COMPILER == COMPILER_MSVC
#	pragma warning(disable : 4091)
#	pragma warning(disable : 4244)
#	pragma warning(disable : 4251)
#	pragma warning(disable : 4267)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4624)
#	pragma warning(disable : 4819)
#	pragma warning(disable : 4996)
#elif COMPILER == COMPILER_CLANG
#	pragma clang diagnostic ignored"-Winconsistent-missing-override"
#else
#endif

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/// VERSION

#define XE_MAJOR_VERSION unsigned char( 0 )
#define XE_MINOR_VERSION unsigned char( 0 )
#define XE_PATCH_VERSION unsigned char( 0 )

#define XE_VERSION ( unsigned int( XE_MAJOR_VERSION << 16 ) | unsigned int( XE_MINOR_VERSION << 8 ) | unsigned int( XE_PATCH_VERSION ) )
#define XE_VERSION_STRING MACRO_TO_STRING( XE_MAJOR_VERSION )"."MACRO_TO_STRING( XE_MINOR_VERSION )"."MACRO_TO_STRING( XE_PATCH_VERSION ) 

/////////////////////////////////////////////////////////////////////////////////////////

#endif // __CONFIG_H__851B77A2_E78D_4E14_91A6_D6317250A486
