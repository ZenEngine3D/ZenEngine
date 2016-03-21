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

class SampleDebugUIInstance : public zenSys::zSampleEngineInstance
{
ZENClassDeclare(SampleDebugUIInstance, zenSys::zEngineInstance);
public:
	virtual bool					Init();
	virtual void					Destroy();
	virtual void					Update();
	virtual bool					IsDone();
	virtual const char*				GetAppName()const;
	

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
	zenRes::zGfxMeshStrip			mrCubeMeshStrip;	

	zenRes::zGfxRenderTarget		mrBackbufferDepth;
	zenRes::zGfxStateRasterizer		mrStateRaster;
	zenRes::zGfxRenderPass			mrRndPassFinal;
};

}

#endif