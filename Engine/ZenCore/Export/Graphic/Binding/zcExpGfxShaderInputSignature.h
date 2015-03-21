#pragma once
#ifndef __zCore_Exp_Gfx_InputStreamBinding_h__
#define __zCore_Exp_Gfx_InputStreamBinding_h__

namespace zcExp
{

class SerialInputSignature_Base : public zcExp::ExportItem
{
ZENClassDeclare(SerialInputSignature_Base, zcExp::ExportItem)
public:
	struct ExportInfo : public zcExp::ExportInfoBase
	{				
		zResID mParentShaderID;
	};
};

zResID CreateGfxInputSignature( const zResID& _ParentShaderID );

}

#include "zcExpGfxShaderInputSignatureDX11.h"


#endif