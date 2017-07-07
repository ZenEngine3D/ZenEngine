#pragma once

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

class SampleRendererInstance : public zenSys::zEngineInstance
{
zenClassDeclare(SampleRendererInstance, zenSys::zEngineInstance);
public:
	virtual bool					Init();
	virtual void					Destroy();
	virtual void					Update();
	virtual bool					IsDone();
	virtual const char*				GetAppName()const;

protected:
	void									UpdateBackbuffers();
	zenMath::Matrix							matWorld[4];
	zenMath::Matrix							matView;
	zenMath::Matrix							matProjection;
	zenMath::Matrix							matProjectionRT;
	
	zenRes::zGfxIndex						mrCubeIndex;	
	zenRes::zGfxStructBuffer<zVec3F>		mrCubeVtxPos;
	zenRes::zGfxStructBuffer<BufferColorUV>	mrCubeVtxColorUv;		
	zenRes::zGfxIndex						mrTriangleIndex;	
	zenRes::zGfxStructBuffer<zVec3F>		mrTriangleVtxPos;
	zenRes::zGfxStructBuffer<BufferColorUV>	mrTriangleVtxColorUv;

	zenRes::zGfxShaderVertex				mrShaderVS;	
	zenRes::zGfxShaderPixel					mrShaderPS;	
	zenRes::zGfxShaderPixel					mrShaderPS2Output;		
	zenRes::zGfxTexture2D					mrTexture;	
	zenRes::zGfxStateSampler				mrSampler;	
	zenRes::zGfxStateSampler				mrSamplerPoint;		
	zenRes::zGfxShaderBinding				mrShaderBind;
	zenRes::zGfxShaderBinding				mrShader2OutputBind;
	
	zenRes::zGfxMeshStrip					mrCube1MeshStrip;
	zenRes::zGfxMeshStrip					mrCube2MeshStrip;
	zenRes::zGfxMesh						mrCube3Mesh;
	zenRes::zGfxMeshStrip					mrTriangleMeshStrip;

	zenRes::zGfxTarget2D					mrRenderToTextureRT1;
	zenRes::zGfxTarget2D					mrRenderToTextureRT2;
	zenRes::zGfxTarget2D					mrRenderToTextureDepth;
	zenRes::zGfxTarget2D					mrBackbufferDepth;	
	zenRes::zGfxStateRaster					mrStateRaster;
	zenRes::zGfxRenderPass					mrRndPassTexture;
	zenRes::zGfxRenderPass					mrRndPassFinal;
};

}
