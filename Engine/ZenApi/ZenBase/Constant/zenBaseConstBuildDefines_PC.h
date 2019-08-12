#pragma once

#ifdef _WINDOWS
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WinSock2.h>
#include <windows.h>
#include <Shellapi.h>
#endif

#define	ZEN_EXPORT_OR_RESOURCE_PC	ZEN_PLATFORM_PC
#define	ZEN_EXPORT_PC				ZEN_BUILD_PC_GAMEORTOOL

#define zenInline					__inline
#define zenInlineNone				__declspec(noinline)

#if ZEN_BUILD_DEBUG
	#define zenInlineForce			__inline
	#define zenInlineNotInDebug		__declspec(noinline)
#else
	#define zenInlineForce			__forceinline
	#define zenInlineNotInDebug		zenInline
#endif

#define ZENAlign(_VAR_,_BYTESALIGN_)	__declspec(align(_BYTESALIGN_)) _VAR_

#ifdef _MSC_VER 
#define zenRestrict						__restrict
#endif