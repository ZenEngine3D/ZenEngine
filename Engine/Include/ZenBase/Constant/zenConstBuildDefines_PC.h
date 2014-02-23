#ifdef _WINDOWS
//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <WinSock2.h>
#include <windows.h>
#include <Shellapi.h>
#endif

#define	AW_EXPORT_OR_RESOURCE_WIN	AW_PLATFORM_PC
#define	AW_EXPORT_WIN				AW_BUILD_PC_GAMEORTOOL
