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
			zenArrayStatic<zenU16>	maIndice16;			//!< 16bits Indices list (Pick 1st one valid)
			zenArrayStatic<zenU32>	maIndice32;			//!< 32bits Indices list
			zenConst::ePrimitiveType	mePrimitiveType;	//!< Type of primitive the indices are for
		};
	};
	
	zenResID CreateGfxIndexBuffer(const zenArrayBase<zenU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	zenResID CreateGfxIndexBuffer(const zenArrayBase<zenU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
}

#include "EExpIndexDX11.h"

#endif
