#pragma once

//=================================================================================================
//! @example SampleGfxGLTL.cpp
//! Test loading GL TL scene into the engine and rendering them
//=================================================================================================

#include <eastl/hash_map.h>
namespace sample
{

namespace GLTL
{
	struct Texture
	{
		zenRes::zGfxTexture2D	mrTexture;
		zString					mName;
	};
	struct Buffer
	{
		zenRes::zGfxBuffer		mrBuffer;
		zString					mName;
	};

	struct Material
	{
		zenRes::zGfxShaderBinding	mrShaderBind;
		zString						mName;
	};

	struct Mesh
	{
		zArrayDyn<zenRes::zGfxMeshStrip>	marStrip;
		zenRes::zGfxMesh					mrMesh;
		zString								mName;
	};
} //namespace GLTL

class SampleGLTLInstance : public zenSys::zSampleEngineInstance
{
zenClassDeclare(SampleGLTLInstance, zenSys::zEngineInstance);
public:
	virtual bool					Init();
	virtual void					Destroy();
	virtual void					Update();
	virtual bool					IsDone(){return false;}
	virtual const char*				GetAppName()const {return "Sample GLTL";}
	
protected:
	void										UpdateBackbuffers();
	zenRes::zGfxTarget2D						mrBackbufferDepth;
	zenRes::zGfxRenderPass						mrRndPassFinal;
	zenRes::zGfxStateRaster						mrStateRaster;
	zenMath::Matrix								matWorld;
	zenMath::Matrix								matView;
	zenMath::Matrix								matProjection;

	zenRes::zGfxShaderVertex					mrShaderVS;	
	zenRes::zGfxShaderPixel						mrShaderPS;		
	zenRes::zGfxShaderBinding					mrShaderBind;

	zArrayDyn<GLTL::Texture>					maSceneTexture;
	zArrayDyn<GLTL::Buffer>						maSceneBuffer;
	//zArrayStatic<GLTL::Material>				maSceneMaterial;
	eastl::hash_map<zU64, zenRes::zGfxIndex>	mdSceneBufferIndex;
	zArrayDyn<GLTL::Mesh>						maSceneMesh;
	
};

}
