#include "zcCore.h"

namespace zcRes
{
	bool GfxRenderTarget::ResourceInit()
	{
		if( mrExportData->mpBackbuffer == NULL )
		{
			mrTexture2D = zcExp::CreateGfxTexture2D(mrExportData->meFormat, zFlagResTexUse(zenConst::keTexCreate_RenderTarget), mrExportData->mvDim );
			if( mrTexture2D.IsValid() )
				return Super::ResourceInit();
		}
		else
			return Super::ResourceInit();

		return false;		
	}

}