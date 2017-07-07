#pragma once

#include zenHeaderRenderer(zcResGfxRenderTarget)

namespace zcRes
{

class GfxTarget2D : protected GfxTarget2D_HAL
{
zenClassDeclare(GfxTarget2D, GfxTarget2D_HAL)
public:
	virtual bool					Initialize();

	bool							IsDepth();
	const zVec2U16&					GetDim();
	const GfxTexture2DRef&			GetTexture2D()const{return mrTexture2D;}

protected:	
	GfxTexture2DRef					mrTexture2D;
	bool							mbNeedResolve;	
};

}
