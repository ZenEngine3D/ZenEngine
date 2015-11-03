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
	zenRes::zGfxShaderPixel			mrShaderPS2Output;		
	zenRes::zGfxTexture2d			mrTexture;	
	zenRes::zGfxSampler				mrSampler;	
	zenRes::zGfxSampler				mrSampler2;		
	zenRes::zGfxShaderBinding		mrShaderBind;
	zenRes::zGfxShaderBinding		mrShader2OutputBind;
	zenRes::zGfxMeshStrip			mrCube3MeshStrip;	

	zenRes::zGfxRenderTarget		mrBackbufferDepth;
	zenRes::zGfxStateBlend			mrStateBlend;
	zenRes::zGfxStateRasterizer		mrStateRaster;
	zenRes::zGfxRenderPass			mrRndPassTexture;
	zenRes::zGfxRenderPass			mrRndPassFinal;
};

}

#endif