#pragma once
#ifndef __zCore_Exp_Gfx_InputStreamBinding_Base_h__
#define __zCore_Exp_Gfx_InputStreamBinding_Base_h__

namespace zcExp
{

struct ExportInfoGfxInputStream : public ExportInfoBase
{			
	zResID	mVertexBufferID;
	zResID	mShaderInputSignatureID;
};

zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID);

}

#include "zcExpGfxInputStreamDX11.h"

#endif