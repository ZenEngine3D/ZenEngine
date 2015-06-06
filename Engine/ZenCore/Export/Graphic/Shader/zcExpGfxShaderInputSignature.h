#pragma once
#ifndef __zCore_Exp_Gfx_InputStreamBinding_h__
#define __zCore_Exp_Gfx_InputStreamBinding_h__

namespace zcExp
{

struct ExportInfoGfxInputSignature : public ExportInfoBase
{
	zResID mParentShaderID;
};

zResID CreateGfxInputSignature( const zResID& _ParentShaderID );

}

#include "zcExpGfxShaderInputSignatureDX11.h"


#endif