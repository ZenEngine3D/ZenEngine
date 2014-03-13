#pragma once
#ifndef __LibExport_Graphics_Indice_h__
#define __LibExport_Graphics_Indice_h__

namespace EExp
{
	class SerialIndex_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialIndex_Base, EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zArrayStatic<zU16>	maIndice16;			//!< 16bits Indices list (Pick 1st one valid)
			zArrayStatic<zU32>	maIndice32;			//!< 32bits Indices list
			zenConst::ePrimitiveType	mePrimitiveType;	//!< Type of primitive the indices are for
		};
	};
	
	zResID CreateGfxIndexBuffer(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	zResID CreateGfxIndexBuffer(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
}

#include "EExpIndexDX11.h"

#endif
