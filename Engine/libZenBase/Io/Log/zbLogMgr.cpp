#include "libZenBase.h"

namespace CMgr { zbLog::ManagerLog Log; }

namespace zbLog
{
// Make sure to match enum eLogType
const char* ManagerLog::ssLogTypeDesc[keLog__Count+1]={
		"General",
		"Game",
		"Critical",
		"Assert",
		"Warning",
		"File I/O",
		"Export Failed",
		"Export Passed",
		"Res. Texture",
		"Res. Material",
		"Res. Geometry",
		"Debug Info"
		""
		};

bool ManagerLog::Load()
{
	Reset();
	return true;
}

bool ManagerLog::Unload()
{
	return true;
}

}   

