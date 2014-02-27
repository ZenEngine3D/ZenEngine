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
	awres::awGfxVertex			rCubeVertex;	
	awres::awGfxIndex			rCubeIndex;	
	awres::awGfxShaderVertex	rShaderVS;	
	awres::awGfxShaderPixel		rShaderPS;	
	awres::awGfxTexture2D		rTexture;	
	awres::awGfxSampler			rSampler;	
	awres::awGfxSampler			rSampler2;	
	awres::awGfxRenderTarget	rRenderColor;
	awres::awGfxRenderTarget	rRenderDepth;
	awres::awGfxView			rView;
	awres::awGfxShaderBinding	rTestShaderBind;
	awres::awGfxMeshStrip		rCube1MeshStrip;
	awres::awGfxMeshStrip		rCube2MeshStrip;
	awres::awGfxMeshStrip		rCube3MeshStrip;
	awres::awGfxMesh			rCube4Mesh;
};

}

#endif