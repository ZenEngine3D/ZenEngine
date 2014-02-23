#pragma once
#ifndef __zenBase_Type_Constant_Version_h__
#define __zenBase_Type_Constant_Version_h__

namespace zen { namespace awconst {

enum eEngineVersion
{
	keEngineVersion_FirstRelease,
	keEngineVersion__Total,
	keEngineVersion__Current=keEngineVersion__Total-1,
	keEngineVersion__Invalid,
};
  

}} // namespace zen, Const

#endif