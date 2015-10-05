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

class SampleRendererInstance : public zenSys::zEngineInstance
{
ZENClassDeclare(SampleRendererInstance, zenSys::zEngineInstance);
public:
	virtual bool Init();
	virtual void Destroy();
	virtual void Update();
	virtual bool IsDone();
protected:
	zenMath::Matrix					matWorld[4];
	zenMath::Matrix					matView;
	zenMath::Matrix					matProjection;
	zenMath::Matrix					matProjectionRT;
	zenRes::zGfxVertex				mrCubeVertex;	
	zenRes::zGfxIndex				mrCubeIndex;	
	zenRes::zGfxShaderVertex		mrShaderVS;	
	zenRes::zGfxShaderPixel			mrShaderPS;	
	zenRes::zGfxTexture2d			mrTexture;	
	zenRes::zGfxSampler				mrSampler;	
	zenRes::zGfxSampler				mrSampler2;		
	zenRes::zGfxShaderBinding		mrTestShaderBind;
	zenRes::zGfxMeshStrip			mrCube1MeshStrip;
	zenRes::zGfxMeshStrip			mrCube2MeshStrip;
	zenRes::zGfxMeshStrip			mrCube3MeshStrip;
	zenRes::zGfxMesh				mrCube4Mesh;

	zenRes::zGfxRenderTarget		mrRenderColor;
	zenRes::zGfxRenderTarget		mrRenderDepth;	
	zenRes::zGfxView				mrView;
	zenRes::zGfxStateBlend			mrStateBlend;
	zenRes::zGfxStateDepthStencil	mrStateDepth;
	zenRes::zGfxStateRasterizer		mrStateRaster;
	zenRes::zGfxRenderPass			mrRndPassTexture;
	zenRes::zGfxRenderPass			mrRndPassFinal;

};

}

#endif