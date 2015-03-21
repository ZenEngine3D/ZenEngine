#pragma once
#ifndef __zCore_Exp_Gfx_InputStreamBinding_Base_h__
#define __zCore_Exp_Gfx_InputStreamBinding_Base_h__

namespace zcExp
{
	class SerialInputStream_Base : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialInputStream_Base,zcExp::ExportItem)
	public:		
		struct ExportInfo : public zcExp::ExportInfoBase
		{			
			zResID	mVertexBufferID;
			zResID	mShaderInputSignatureID;
		};
	};

	zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID);
}

#include "zcExpGfxInputStreamBindingDX11.h"

#endif