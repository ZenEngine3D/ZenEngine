#include "zcCore.h"

namespace zcRes
{

bool GfxRenderPass_DX12::Initialize()
{		
	mrStateBlend		= mBlendStateID;
	mrStateDepthStencil	= mDepthStencilStateID;
	mrStateRaster		= mRasterStateID;
	mrStateView			= mViewStateID;
	return true;
}

}
