#pragma once
#ifndef __zCore_Res_Gfx_GfxRenderTarget_h__
#define __zCore_Res_Gfx_GfxRenderTarget_h__

#include ZENHeaderRenderer(zcResGfxRenderTarget)

namespace zcRes
{

class GfxTarget2D : protected GfxTarget2DHAL
{
ZENClassDeclare(GfxTarget2D, GfxTarget2DHAL)
public:
	using							Super::Clear;
	virtual bool					Initialize();

	bool							IsDepth();
	const zVec2U16&					GetDim();
	const GfxTexture2dRef&			GetTexture2D()const{return mrTexture2D;}

protected:	
	GfxTexture2dRef					mrTexture2D;
	bool							mbNeedResolve;	
};

}

#endif
