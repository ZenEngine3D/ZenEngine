#pragma once
#ifndef __LibExport_Graphics_Indice_h__
#define __LibExport_Graphics_Indice_h__

namespace EExp
{
	class SerialIndex_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialIndex_Base, EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			awArrayStatic<awU16>	maIndice16;			//!< 16bits Indices list (Pick 1st one valid)
			awArrayStatic<awU32>	maIndice32;			//!< 32bits Indices list
			awconst::ePrimitiveType	mePrimitiveType;	//!< Type of primitive the indices are for
		};
	};
	
	awResourceID CreateGfxIndexBuffer(const awArrayBase<awU16>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
	awResourceID CreateGfxIndexBuffer(const awArrayBase<awU32>& _Indices, awconst::ePrimitiveType _ePrimitiveType);
}

#include "EExpIndexDX11.h"

#endif
