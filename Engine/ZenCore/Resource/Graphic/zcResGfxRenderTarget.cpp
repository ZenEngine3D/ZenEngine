#include "zcCore.h"

namespace zcRes
{
	bool GfxRenderTarget::ResourceInit()
	{
		if( mrResData->mpBackbuffer == NULL )
		{
			mrTexture2D = zcExp::CreateGfxTexture2D(mrResData->meFormat, zFlagResTexUse(zenConst::keTexCreate_RenderTarget), mrResData->mvDim );
			if( mrTexture2D.IsValid() )
				return Super::ResourceInit();
		}
		else
			return Super::ResourceInit();

		return false;		
	}

}