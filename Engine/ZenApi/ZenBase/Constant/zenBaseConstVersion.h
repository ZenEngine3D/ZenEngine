#pragma once
#ifndef __zenApi_Base_Type_Constant_Version_h__
#define __zenApi_Base_Type_Constant_Version_h__

namespace zen { namespace zenConst {

enum eEngineVersion
{
	keEngineVersion__None,
	keEngineVersion_Initial,
	keEngineVersion__Total,
	keEngineVersion__Invalid,
	keEngineVersion__Current=keEngineVersion__Total-1,	
};
  

}} // namespace zen, Const

#endif
