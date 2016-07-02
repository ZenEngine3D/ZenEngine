#pragma once
#ifndef __zCore_Res_Gfx_Binding_DX11_h__
#define __zCore_Res_Gfx_Binding_DX11_h__

namespace zcRes
{
//! @todo Engin : Rethink proxy, use class parenting instead of new object? Used for Hardware Abstraction, not multihreading?

	//=============================================================================================
	//! @class	Binding between a vertex shader and a VertexBuffer.
	//!			Needed for proper mapping of vertex stream into Shader.	
	//=============================================================================================
	class GfxInputStreamProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxInputStreamProxy_DX11, zRefCounted)
	public:
															GfxInputStreamProxy_DX11();
		virtual												~GfxInputStreamProxy_DX11();
		bool												Initialize(class GfxInputStream& _Owner);

		zcRes::GfxVertexRef									mrVertexProxy;
		zcRes::GfxInputSignatureRef							mrSignatureProxy;
		ID3D11InputLayout*									mpInputLayout;	//!< Contain DX object for vertex input remapping
		ZENDbgCode(class GfxInputStream*					mpOwner);
	};


	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStripProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxMeshStripProxy_DX11, zRefCounted)
	public:
															GfxMeshStripProxy_DX11();
		virtual												~GfxMeshStripProxy_DX11();
		bool												Initialize(class GfxMeshStrip& _Owner);

		void												SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void												SetValue(const zenRes::zShaderParameter& _Value);
		void												SetValue(const zHash32& _hParamName, const float& _fValue);
		void												SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture);
		void												SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler);
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler);

		GfxIndexRef											mrIndexBufferProxy;
		GfxInputStreamRef									mrInputStreamProxy;			//!< Reference to input streams binding
		GfxShaderBindingRef									mrShaderBindingProxy;		//!< Reference to shader binding infos 
		zArrayStatic<GfxShaderParamRef>						marShaderParamProxy;		//!< Array of all ShaderParam used by all Shaders stage
		zArrayStatic<zArrayStatic<GfxTexture2dRef>>			marTextureProxy;			//!< Per Shader stage texture input (Array sub Index = Slot)
		zArrayStatic<zArrayStatic<GfxSamplerRef>>			marGfxSamplerProxy;			//!< Per Shader stage texture input (Array sub Index = Slot)			
		zU32												muVertexFirst;
		zU32												muIndexFirst;
		zU32												muIndexCount;
		ZENDbgCode(class GfxMeshStrip*						mpOwner);
	};

	
	//=============================================================================================
	//! @class	Bind together all strip of a Mesh
	//=============================================================================================
	class GfxMeshProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxMeshProxy_DX11, zRefCounted)
	public:
															GfxMeshProxy_DX11();
		virtual												~GfxMeshProxy_DX11();
		bool												Initialize(class GfxMesh& _Owner);
		void												SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void												SetValue(const zenRes::zShaderParameter& _Value);
		void												SetValue(const zHash32& _hParamName, const float& _fValue);
		void												SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture);
		void												SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler);
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dRef _rTexture, GfxSamplerRef _rSampler);

		zArrayStatic<GfxMeshStripRef>						marProxGfxMeshStrip;	//!< 
		ZENDbgCode(class GfxMesh*							mpOwner);
	};

	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBindingProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxShaderBindingProxy_DX11, zRefCounted)
	public:
															GfxShaderBindingProxy_DX11();
		virtual												~GfxShaderBindingProxy_DX11();
		bool												Initialize(class GfxShaderBinding& _Owner);
		GfxShaderVertexRef									mrProxShaderVertex;		//! @todo clean have common interface to shader stage again? Pointer to proxy instead?
		GfxShaderPixelRef									mrProxShaderPixel;			
		zMap<zU32>::Key32									mdBufferPerParam;		//!< Bitmask of each ShaderParam containing this parameter name
		zMap<GfxShaderBindingResData::TextureSlot>::Key32	mdStageSlotPerTexture;	//!< Texture Index of each Shader stage using this parameter name
		zArrayStatic<zU32>									maStagePerParamDef;		//!< Array of Bitfield of valid shader stage per ShaderParam (match mSerial.maParamDef)
		ZENDbgCode(class GfxShaderBinding*					mpOwner);
	};

}

#endif
