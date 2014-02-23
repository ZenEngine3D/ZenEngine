#pragma once
#ifndef __LibExport_Graphics_InputStreamBinding_h__
#define __LibExport_Graphics_InputStreamBinding_h__

namespace EExp
{

class SerialInputSignature_Base : public EExp::ExportItem
{
AWClassDeclare(SerialInputSignature_Base, EExp::ExportItem)
public:
	struct ExportInfo : public EExp::ExportInfoBase
	{				
		awResourceID mParentShaderID;
	};
};

awResourceID CreateGfxInputSignature( const awResourceID& _ParentShaderID );

}

#include "EExpShaderInputSignatureDX11.h"


#endif