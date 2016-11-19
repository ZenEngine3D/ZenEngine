#include "zcCore.h"

//SF DX12

namespace zcRes
{

bool GfxRenderPassHAL_DX12::Initialize()
{		
	mrStateBlend		= mBlendStateID;
	mrStateDepthStencil	= mDepthStencilStateID;
	mrStateRaster		= mRasterStateID;
	mrStateView			= mViewStateID;
	return true;
}

}
