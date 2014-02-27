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
			zenResID	mVertexBufferID;
			zenResID	mShaderInputSignatureID;
		};
	};

	zenResID CreateGfxInputStream(zenResID _VertexBufferID, zenResID _ShaderInputSignatureID);
}

#include "EExpInputStreamBindingDX11.h"

#endif