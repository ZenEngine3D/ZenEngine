#include "zcCore.h"

namespace zcRes
{

bool GfxTarget2D::Initialize()
{
	mrTexture2D = mTextureID;
	return Super::Initialize();
}

bool GfxTarget2D::IsDepth()
{ 
	return zcMgr::GfxRender.IsDepth(meFormat);
}
	
const zVec2U16&	GfxTarget2D::GetDim()
{ 
	return mvDim; 
}

}