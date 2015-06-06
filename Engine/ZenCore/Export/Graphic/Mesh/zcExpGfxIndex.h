#pragma once
#ifndef __zCore_Exp_Gfx_Indice_h__
#define __zCore_Exp_Gfx_Indice_h__

namespace zcExp
{
	struct ExportInfoGfxIndex : public ExportInfoBase
	{			
		zArrayStatic<zU16>			maIndice16;			//!< 16bits Indices list (Pick 1st one valid)
		zArrayStatic<zU32>			maIndice32;			//!< 32bits Indices list
		zenConst::ePrimitiveType	mePrimitiveType;	//!< Type of primitive the indices are for
	};
	
	zResID CreateGfxIndexBuffer(const zArrayBase<zU16>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
	zResID CreateGfxIndexBuffer(const zArrayBase<zU32>& _Indices, zenConst::ePrimitiveType _ePrimitiveType);
}

#include "zcExpGfxIndexDX11.h"

#endif
