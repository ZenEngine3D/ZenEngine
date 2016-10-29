#pragma once
#ifndef __zCore_Res_Gfx_Binding_h__
#define __zCore_Res_Gfx_Binding_h__

#include ZENHeaderRenderer(zcResGfxBinding)

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStrip : protected GfxMeshStripHAL
	{
	zenClassDeclare(GfxMeshStrip, GfxMeshStripHAL)
	public:
		using Super::SetValue;
		using Super::SetResource;
		zenInline zU32 GetVertexFirst()const{return muVertexFirst;}
		zenInline zU32 GetIndexFirst()const{return muIndexFirst;}
		zenInline zU32 GetIndexCount()const{return muIndexCount;}
	};

	//=============================================================================================
	//! @class	List of MeshStrip resource to generate entire mesh
	//=============================================================================================
	class GfxMesh : protected GfxMeshHAL
	{
	zenClassDeclare(GfxMesh, GfxMeshHAL)
	public:		
		const zArrayStatic<GfxMeshStripRef>& GetMeshStrips()const;	
		using Super::SetValue;
		using Super::SetResource;
	};

	class GfxShaderBinding : protected GfxShaderBindingHAL
	{
	zenClassDeclare(GfxShaderBinding, GfxShaderBindingHAL)
	using Super::CreateShaderParam;
	};
}

#endif
