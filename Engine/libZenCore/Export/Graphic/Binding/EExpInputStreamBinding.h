#pragma once
#ifndef __LibExport_Graphics_InputStreamBinding_Base_h__
#define __LibExport_Graphics_InputStreamBinding_Base_h__

namespace EExp
{
	class SerialInputStream_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialInputStream_Base,EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			awResourceID	mVertexBufferID;
			awResourceID	mShaderInputSignatureID;
		};
	};

	awResourceID CreateGfxInputStream(awResourceID _VertexBufferID, awResourceID _ShaderInputSignatureID);
}

#include "EExpInputStreamBindingDX11.h"

#endif