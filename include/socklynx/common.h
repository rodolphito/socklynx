#ifndef SL_PLATFORM_H
#define SL_PLATFORM_H


/* defines that can force compliance with exotic platforms and C implementations */
//#define SL_NO_ASSERT
//#define SL_NO_STATIC_ASSERT
//#define SL_NO_STDBOOL
//#define SL_NO_STDINT
//#define SL_FORCE_64
//#define SL_FORCE_32
//#define SL_FORCE_PLATFORM_NONE
//#define SL_FORCE_PLATFORM_PS4
//#define SL_FORCE_PLATFORM_XBONE
//#define SL_FORCE_PLATFORM_SWITCH
//#define SL_FORCE_C_NONE


#if !defined(SL_FORCE_32) && (defined(SL_FORCE_64) || defined(__x86_64__) || defined(_M_AMD64) || defined(__aarch64__) || defined(__ia64__) || defined(__powerpc64__))
#	define SL_64 1
#else
#	define SL_32 1
#endif


#if defined(SL_FORCE_PLATFORM_NONE)
#	define SL_PLATFORM_NONE 1
#elif defined(SL_FORCE_PLATFORM_PS4)
#	define SL_PLATFORM_PS4 1
#elif defined(SL_FORCE_PLATFORM_XBONE)
#	define SL_PLATFORM_XBONE 1
#elif defined(SL_FORCE_PLATFORM_SWITCH)
#	define SL_PLATFORM_SWITCH 1
#elif defined(__CYGWIN32__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#	define SL_PLATFORM_WINDOWS 1
#elif __APPLE__
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR
#		define SL_PLATFORM_IOS 1
#	elif TARGET_OS_IPHONE
#		define SL_PLATFORM_IOS 1
#	elif TARGET_OS_MAC
#		define SL_PLATFORM_OSX 1
#	else
#	endif
#elif __ANDROID__
#	define SL_PLATFORM_ANDROID 1
#elif __linux__
#	define SL_PLATFORM_POSIX 1
#elif __unix__
#	define SL_PLATFORM_POSIX 1
#elif defined(_POSIX_VERSION)
#	define SL_PLATFORM_POSIX 1
#else
#endif


#if !defined(SL_FORCE_C_NONE) && defined(_MSC_VER)
#	define SL_C_MSC 1
#elif !defined(SL_FORCE_C_NONE) && defined(__GNUC__)
#	define SL_C_GCC 1
#elif !defined(SL_FORCE_C_NONE) && defined(__clang__)
#	define SL_C_CLANG 1
#else
#	define SL_C_NONE 1
#endif


#if SL_C_MSC 
#	define SL_CALL __cdecl
#	define SL_INLINE __forceinline

#	ifdef SL_EXPORTS
#		define SL_API __declspec(dllexport)
#	else
#		define SL_API __declspec(dllimport)
#	endif
#elif SL_C_GCC || SL_C_CLANG
#	define SL_INLINE __attribute__((always_inline))
#	define SL_CALL
#	define SL_API
#else
#	define SL_INLINE 
#	define SL_CALL
#	define SL_API
#endif


#ifndef SL_NO_STDBOOL
#    include <stdbool.h>
#elif SL_C_GCC && !defined(__STRICT_ANSI__)
#	define _Bool bool
#endif

#ifndef SL_NO_STDINT
#	include <stdint.h>
#	ifdef SL_PLATFORM_ANDROID
#		include <limits.h>
#	endif
#else

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
#	if SL_64
typedef long long int int64_t;
#	else
typedef long int int64_t;
#	endif

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef unsigned int uint32_t;

#	if SL_64
typedef unsigned long long int uint64_t;
#	else
typedef unsigned long int uint64_t;
#	endif

#	if SL_64
typedef long long int intptr_t;
typedef unsigned long long int uintptr_t;
#	else
typedef long int intptr_t;
typedef unsigned long int uintptr_t;
#	endif

#	if SL_64
#		define __INT64_C(c) c##L
#		define __UINT64_C(c) c##UL
#	else
#		define __INT64_C(c) c##LL
#		define __UINT64_C(c) c##ULL
#	endif

#	define INT8_MIN (-128)
#	define INT16_MIN (-32767 - 1)
#	define INT32_MIN (-2147483647 - 1)
#	define INT64_MIN (-__INT64_C(9223372036854775807) - 1)
#	define INT8_MAX (127)
#	define INT16_MAX (32767)
#	define INT32_MAX (2147483647)
#	define INT64_MAX (__INT64_C(9223372036854775807))
#	define UINT8_MAX (255)
#	define UINT16_MAX (65535)
#	define UINT32_MAX (4294967295U)
#	define UINT64_MAX (__UINT64_C(18446744073709551615))

#endif


#ifdef SL_NO_STATIC_ASSERT
#	define SL_STATIC_ASSERT(cond)
#else
#	define SL_CONCAT(A, B) A ## B
#	define SL_STATIC_ASSERT0(cond, msg) typedef char SL_CONCAT(static_assertion_, msg)[(!!(cond)) * 2 - 1]
#	define SL_STATIC_ASSERT1(cond, line) SL_STATIC_ASSERT0(cond, SL_CONCAT(at_line_, line))
#	define SL_STATIC_ASSERT(cond) SL_STATIC_ASSERT1(cond, __LINE__)
#endif


SL_STATIC_ASSERT(sizeof(uint64_t) == 8);
SL_STATIC_ASSERT(sizeof(uint32_t) == 4);
SL_STATIC_ASSERT(sizeof(uint16_t) == 2);
SL_STATIC_ASSERT(sizeof(uint8_t) == 1);
SL_STATIC_ASSERT(sizeof(int64_t) == 8);
SL_STATIC_ASSERT(sizeof(int32_t) == 4);
SL_STATIC_ASSERT(sizeof(int16_t) == 2);
SL_STATIC_ASSERT(sizeof(int8_t) == 1);
SL_STATIC_ASSERT(sizeof(uintptr_t) == sizeof(void *));
SL_STATIC_ASSERT(sizeof(intptr_t) == sizeof(void *));
SL_STATIC_ASSERT(sizeof(char) == 1);

#endif
