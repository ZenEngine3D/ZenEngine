#pragma once
#ifndef __zCore_Res_Gfx_Binding_DX11_h__
#define __zCore_Res_Gfx_Binding_DX11_h__

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding between a vertex shader and a VertexBuffer.
	//!			Needed for proper mapping of vertex stream into Shader.	
	//=============================================================================================
	class GfxInputStreamProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxInputStreamProxy_DX11, zRefCountedAutoDel)
	public:
												GfxInputStreamProxy_DX11();
		virtual									~GfxInputStreamProxy_DX11();												
		bool									Initialize(class GfxInputStream& _Owner);
	
		zcRes::GfxVertexProxyRef				mrVertexProxy;		
		zcRes::GfxInputSignatureProxyRef		mrSignatureProxy;
		ID3D11InputLayout*						mpInputLayout;	//!< Contain DX object for vertex input remaping
		ZENDbgCode(class GfxInputStream*	mpOwner);
	};


	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStripProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxMeshStripProxy_DX11, zRefCountedAutoDel)
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
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture );
		void												SetValue(const zHash32& _hTextureName, GfxSamplerProxyRef _rSampler );
		void												SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture, GfxSamplerProxyRef _rSampler );
		
		GfxIndexProxyRef									mrIndexBufferProxy;
		GfxInputStreamProxyRef								mrInputStreamProxy;			//!< Reference to input streams binding
		GfxShaderBindingProxyRef							mrShaderBindingProxy;		//!< Reference to shader binding infos 
		zArrayStatic<GfxShaderParamProxyRef>				marShaderParamProxy;		//!< Array of all ShaderParam used by all Shaders stage
		zArrayStatic<zArrayStatic<GfxTexture2dProxyRef>>	marTextureProxy;			//!< Per Shader stage texture input (Array sub Index = Slot)
		zArrayStatic<zArrayStatic<GfxSamplerProxyRef>>		marGfxSamplerProxy;			//!< Per Shader stage texture input (Array sub Index = Slot)			
		zU32												muIndexFirst;
		zU32												muIndexCount;
		ZENDbgCode(class GfxMeshStrip*				mpOwner);
	};

	
	//=============================================================================================
	//! @class	Bind together all strip of a Mesh
	//=============================================================================================
	class GfxMeshProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxMeshProxy_DX11, zRefCountedAutoDel)
	public:
																GfxMeshProxy_DX11();
		virtual													~GfxMeshProxy_DX11();
		bool													Initialize(class GfxMesh& _Owner);
		void													SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void													SetValue(const zenRes::zShaderParameter& _Value);
		void													SetValue(const zHash32& _hParamName, const float& _fValue);
		void													SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void													SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void													SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void													SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void													SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture );
		void													SetValue(const zHash32& _hTextureName, GfxSamplerProxyRef _rSampler );
		void													SetValue(const zHash32& _hTextureName, GfxTexture2dProxyRef _rTexture, GfxSamplerProxyRef _rSampler );
		
		zArrayStatic<GfxMeshStripProxyRef>						marProxGfxMeshStrip;	//!< 
		ZENDbgCode(class GfxMesh*							mpOwner);
	};

	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBindingProxy_DX11 : public zRefCountedAutoDel
	{
	ZENClassDeclare(GfxShaderBindingProxy_DX11, zRefCountedAutoDel)
	public:
																GfxShaderBindingProxy_DX11();
		virtual													~GfxShaderBindingProxy_DX11();
		bool													Initialize(class GfxShaderBinding& _Owner);
		GfxShaderVertexProxyRef									mrProxShaderVertex;	//! @todo clean have common interface to shader stage again? Pointer to proxy instead?
		GfxShaderPixelProxyRef									mrProxShaderPixel;			
		zMap<zU32>::Key32										mdBufferPerParam;		//!< Bitmask of each ShaderParam containing this parameter name
		zMap<GfxShaderBindingResData::TextureSlot>::Key32	mdStageSlotPerTexture;	//!< Texture Index of each Shader stage using this parameter name
		zArrayStatic<zU32>										maStagePerParamDef;		//!< Array of Bitfield of valid shader stage per ShaderParam (match mSerial.maParamDef)
		ZENDbgCode(class GfxShaderBinding*				mpOwner);
	};

}

#endif
