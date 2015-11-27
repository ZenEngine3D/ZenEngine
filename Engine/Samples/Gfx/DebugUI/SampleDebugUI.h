#pragma once
#ifndef __Sample_Renderer_h__
#define __Sample_Renderer_h__

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================

namespace sample
{

struct SimpleVertex
{
	zVec3F Pos;
	zVec2F Tex;
};

class SampleDebugUIInstance : public zenSys::zEngineInstance
{
ZENClassDeclare(SampleDebugUIInstance, zenSys::zEngineInstance);
public:
	virtual bool Init();
	virtual void Destroy();
	virtual void Update();
	virtual bool IsDone();

protected:
	void							UpdateBackbuffers();
	zenMath::Matrix					matWorld;
	zenMath::Matrix					matView;
	zenMath::Matrix					matProjection;
	zenRes::zGfxVertex				mrCubeVertex;	
	zenRes::zGfxIndex				mrCubeIndex;	
	zenRes::zGfxShaderVertex		mrShaderVS;	
	zenRes::zGfxShaderPixel			mrShaderPS;	
	zenRes::zGfxTexture2d			mrTexture;	
	zenRes::zGfxSampler				mrSampler;	
	zenRes::zGfxSampler				mrSampler2;		
	zenRes::zGfxShaderBinding		mrShaderBind;
	zenRes::zGfxMeshStrip			mrCube3MeshStrip;	

	zenRes::zGfxRenderTarget		mrBackbufferDepth;
	zenRes::zGfxStateRasterizer		mrStateRaster;
	zenRes::zGfxRenderPass			mrRndPassFinal;

	zenRes::zGfxStateRasterizer		mrUIRaster;  //! need to keep ref on it, untillref counting with only proxy fixed
	zenRes::zGfxRenderPass			mrUIRndPass;
	zenRes::zGfxTexture2d			mrUITexture;
	zenRes::zGfxVertex				mrUIVertex;
	zenRes::zGfxIndex				mrUIIndex;
	zenRes::zGfxShaderVertex		mrUIVS;
	zenRes::zGfxShaderPixel			mrUIPS;
	zenRes::zGfxShaderBinding		mrUIShaderBind;
	zenRes::zGfxMesh				mrUIMesh;
	zenMath::Matrix					matUIOrthographic;
};

}

#endif