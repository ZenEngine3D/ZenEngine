#include "libZenBase.h"

namespace zen { namespace zenIO
{   

const zWString& GetRoot()
{
	return zbMgr::File.GetRoot();
}

const zWString& GetRootPackage()
{
	return zbMgr::File.GetRootPackage();
}

}} //namespace zen { namespace zenMem
