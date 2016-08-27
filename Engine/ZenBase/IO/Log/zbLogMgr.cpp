#include "zbBase.h"

namespace zbMgr 
{ 
	zbLog::ManagerLog Log; 
}

namespace zbLog
{
// Make sure to match enum eLogType
const char* ManagerLog::ssLogTypeDesc[zenConst::keLog__Count+1]={
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
		"" //Last entry empty
		};

bool ManagerLog::Load()
{
	Reset();
	zenStaticAssertMsg(zenArrayCount(ManagerLog::ssLogTypeDesc) == zenConst::keLog__Count+1, "ssLogTypeDesc  must have as many entries as the enum + 1"); 
	return true;
}

bool ManagerLog::Unload()
{
	return true;
}

}   

