#pragma once
#ifndef __zCore_Res_Gfx_Binding_h__
#define __zCore_Res_Gfx_Binding_h__

#include ZENHeaderRenderer(zcResGfxBinding)

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding between a vertex shader and a VertexBuffer.
	//!			Needed for proper mapping of vertex stream into Shader.	
	//=============================================================================================
	class GfxInputStream : public GfxInputStreamHAL
	{
	ZENClassDeclare(GfxInputStream, GfxInputStreamHAL)
	};

	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStrip : protected GfxMeshStripHAL
	{
	ZENClassDeclare(GfxMeshStrip, GfxMeshStripHAL)
	public:
		using Super::SetValue;
		ZENInline zU32 GetVertexFirst()const{return muVertexFirst;}
		ZENInline zU32 GetIndexFirst()const{return muIndexFirst;}
		ZENInline zU32 GetIndexCount()const{return muIndexCount;}
	};

	//=============================================================================================
	//! @class	List of MeshStrip resource to generate entire mesh
	//=============================================================================================
	class GfxMesh : protected GfxMeshHAL
	{
	ZENClassDeclare(GfxMesh, GfxMeshHAL)
	public:		
		const zArrayStatic<GfxMeshStripRef>&	GetMeshStrips()const;	
		using Super::SetValue;
	};

	class GfxShaderBinding : protected GfxShaderBindingHAL
	{
	ZENClassDeclare(GfxShaderBinding, GfxShaderBindingHAL)
	using Super::CreateShaderParam;
	};
}

#endif
