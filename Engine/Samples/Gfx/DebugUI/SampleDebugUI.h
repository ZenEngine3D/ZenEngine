#pragma once
#ifndef __Sample_Renderer_h__
#define __Sample_Renderer_h__

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================

namespace sample
{

struct BufferColorUV
{
	zVec4U8	Color;
	zVec2F	UV;
};

class SampleDebugUIInstance : public zenSys::zSampleEngineInstance
{
zenClassDeclare(SampleDebugUIInstance, zenSys::zEngineInstance);
public:
	virtual bool					Init();
	virtual void					Destroy();
	virtual void					Update();
	virtual bool					IsDone();
	virtual const char*				GetAppName()const;
	

protected:
	void									UpdateBackbuffers();
	zenMath::Matrix							matWorld;
	zenMath::Matrix							matView;
	zenMath::Matrix							matProjection;
	zenRes::zGfxIndex						mrCubeIndex;	
	zenRes::zGfxStructBuffer<zVec3F>		mrVertexBufferPos;
	zenRes::zGfxStructBuffer<BufferColorUV>	mrVertexBufferColorUv;

	zenRes::zGfxShaderVertex				mrShaderVS;	
	zenRes::zGfxShaderPixel					mrShaderPS;	
	zenRes::zGfxTexture2d					mrTexture;	
	zenRes::zGfxSampler						mrSampler;	
	zenRes::zGfxShaderBinding				mrShaderBind;
	zenRes::zGfxMeshStrip					mrCubeMeshStrip;	

	zenRes::zGfxTarget2D					mrBackbufferDepth;
	zenRes::zGfxStateRaster					mrStateRaster;
	zenRes::zGfxRenderPass					mrRndPassFinal;
};

}

#endif