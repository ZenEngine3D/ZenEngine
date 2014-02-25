#include "libZenCore.h"

namespace EExp
{
	bool SerialRenderTarget_Base::ExportEnd()
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);		
		meFormat	= pExportInfo->meFormat;
		mvDim		= pExportInfo->mvDim;
		mbSRGB		= pExportInfo->mbSRGB;
		return Super::ExportEnd();
	}

	//=================================================================================================
	//! @brief		Create a new RenderTarget Resource
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param _eFormat			- Format of raw data
	//! @param _vDim			- Width/Height of input image
	//! @param _bRgb			- If we're rendering to a srgb target	
	//! @return 				- Unique zenResID of created Resource
	//=================================================================================================
	zenResID CreateGfxRenderTarget(awconst::eTextureFormat _eFormat, zenVec2U16 _vDim, bool _bSrgb)
	{		
		static zbMem::PoolAllocator sMemPool("Pool RenderTarget", sizeof(SerialRenderTarget_Base::ExportInfo), 1, 5 );
		SerialRenderTarget_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialRenderTarget_Base::ExportInfo;		
		pExportInfo->meFormat								= _eFormat;
		pExportInfo->mvDim									= _vDim;
		pExportInfo->mbSRGB									= _bSrgb;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, awconst::keResType_GfxRenderTarget, pExportInfo );
	}

}

