#ifdef _WINDOWS
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WinSock2.h>
#include <windows.h>
#include <Shellapi.h>
#endif

#define	ZEN_EXPORT_OR_RESOURCE_PC	AW_PLATFORM_PC
#define	ZEN_EXPORT_PC				AW_BUILD_PC_GAMEORTOOL

#define ZENInline					__inline
#define ZENForceDebugInline			__forceinline
#if !AW_BUILD_DEBUG
	#define ZENForceInline			__forceinline
#else
	#define ZENForceInline			ZENInline
#endif