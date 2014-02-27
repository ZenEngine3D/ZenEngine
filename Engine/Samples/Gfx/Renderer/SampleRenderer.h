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
	zenVec3F Pos;
	zenVec2F Tex;
};

class SampleRendererInstance : public awsys::awEngineInstance
{
ZENClassDeclare(SampleRendererInstance, awsys::awEngineInstance);
public:
	virtual bool Init();
	virtual void Destroy();
	virtual void Update();
	virtual bool IsDone();
protected:
	zenMath::Matrix				matWorld[4];
	zenMath::Matrix				matView;
	zenMath::Matrix				matProjection;
	zenMath::Matrix				matProjectionRT;
	zenRes::GfxVertex			rCubeVertex;	
	zenRes::GfxIndex			rCubeIndex;	
	zenRes::GfxShaderVertex	rShaderVS;	
	zenRes::GfxShaderPixel		rShaderPS;	
	zenRes::GfxTexture2D		rTexture;	
	zenRes::GfxSampler			rSampler;	
	zenRes::GfxSampler			rSampler2;	
	zenRes::GfxRenderTarget	rRenderColor;
	zenRes::GfxRenderTarget	rRenderDepth;
	zenRes::GfxView			rView;
	zenRes::GfxShaderBinding	rTestShaderBind;
	zenRes::GfxMeshStrip		rCube1MeshStrip;
	zenRes::GfxMeshStrip		rCube2MeshStrip;
	zenRes::GfxMeshStrip		rCube3MeshStrip;
	zenRes::GfxMesh			rCube4Mesh;
};

}

#endif