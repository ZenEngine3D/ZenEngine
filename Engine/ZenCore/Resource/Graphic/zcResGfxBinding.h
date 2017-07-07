#pragma once

#include zenHeaderRenderer(zcResGfxBinding)

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStrip : protected GfxMeshStrip_HAL
	{
	zenClassDeclare(GfxMeshStrip, GfxMeshStrip_HAL)
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
	class GfxMesh : protected GfxMesh_HAL
	{
	zenClassDeclare(GfxMesh, GfxMesh_HAL)
	public:		
		const zArrayStatic<GfxMeshStripRef>& GetMeshStrips()const;	
		using Super::SetValue;
		using Super::SetResource;
	};

	class GfxShaderBinding : protected GfxShaderBinding_HAL
	{
	zenClassDeclare(GfxShaderBinding, GfxShaderBinding_HAL)
	using Super::CreateShaderParam;
	};
}
