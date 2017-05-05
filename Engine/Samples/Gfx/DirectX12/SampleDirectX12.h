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

class SampleDirextX12Instance : public zenSys::zSampleEngineInstance
{
zenClassDeclare(SampleDirextX12Instance, zenSys::zEngineInstance);
public:
	virtual bool					Init();
	virtual void					Destroy();
	virtual void					Update();
	virtual bool					IsDone();
	virtual const char*				GetAppName()const;
	
protected:
	void									UpdateBackbuffers();
	zenRes::zGfxIndex						mrCubeIndex;
	zenRes::zGfxStructBuffer<zVec3F>		mrCubeVtxPos;
	zenRes::zGfxStructBuffer<zVec2F>		mrCubeVtxUv;

	zenRes::zGfxIndex						mrTriangleIndex;
	zenRes::zGfxStructBuffer<zVec3F>		mrVBufferTrianglePos;
	zenRes::zGfxStructBuffer<zVec2F>		mrVBufferTriangleUv;

	zenRes::zGfxShaderVertex				mrShaderVS;	
	zenRes::zGfxShaderPixel					mrShaderPS;	
	zenRes::zGfxTexture2D					mrTexture;	
	zenRes::zGfxStateSampler				mrSampler;	
	zenRes::zGfxShaderBinding				mrShaderBind;
	zenRes::zGfxMeshStrip					mrCubeMeshStrip;

	zenRes::zGfxTarget2D					mrBackbufferDepth;
	zenRes::zGfxStateRaster					mrStateRaster;
	zenRes::zGfxRenderPass					mrRndPassFinal;
};

}

#endif