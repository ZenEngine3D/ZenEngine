#pragma once
#ifndef __LibExport_Graphics_InputStreamBinding_h__
#define __LibExport_Graphics_InputStreamBinding_h__

namespace EExp
{

class SerialInputSignature_Base : public EExp::ExportItem
{
ZENClassDeclare(SerialInputSignature_Base, EExp::ExportItem)
public:
	struct ExportInfo : public EExp::ExportInfoBase
	{				
		zenResID mParentShaderID;
	};
};

zenResID CreateGfxInputSignature( const zenResID& _ParentShaderID );

}

#include "EExpShaderInputSignatureDX11.h"


#endif