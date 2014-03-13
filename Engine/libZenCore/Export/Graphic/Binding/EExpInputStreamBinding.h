#pragma once
#ifndef __LibExport_Graphics_InputStreamBinding_Base_h__
#define __LibExport_Graphics_InputStreamBinding_Base_h__

namespace EExp
{
	class SerialInputStream_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialInputStream_Base,EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zResID	mVertexBufferID;
			zResID	mShaderInputSignatureID;
		};
	};

	zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID);
}

#include "EExpInputStreamBindingDX11.h"

#endif