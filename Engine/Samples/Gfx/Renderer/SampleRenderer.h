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
	zenRes::zGfxVertex			rCubeVertex;	
	zenRes::zGfxIndex			rCubeIndex;	
	zenRes::zGfxShaderVertex	rShaderVS;	
	zenRes::zGfxShaderPixel		rShaderPS;	
	zenRes::zGfxTexture2D		rTexture;	
	zenRes::zGfxSampler			rSampler;	
	zenRes::zGfxSampler			rSampler2;	
	zenRes::zGfxRenderTarget	rRenderColor;
	zenRes::zGfxRenderTarget	rRenderDepth;
	zenRes::zGfxView			rView;
	zenRes::zGfxShaderBinding	rTestShaderBind;
	zenRes::zGfxMeshStrip		rCube1MeshStrip;
	zenRes::zGfxMeshStrip		rCube2MeshStrip;
	zenRes::zGfxMeshStrip		rCube3MeshStrip;
	zenRes::zGfxMesh			rCube4Mesh;
};

}

#endif