#include "zcCore.h"

namespace zcRes
{

bool GfxRenderPassHAL_DX11::Initialize()
{		
	mrStateBlend		= mBlendStateID;
	mrStateDepthStencil	= mDepthStencilStateID;
	mrStateRaster		= mRasterStateID;
	mrStateView			= mViewStateID;
	return true;
}

}
