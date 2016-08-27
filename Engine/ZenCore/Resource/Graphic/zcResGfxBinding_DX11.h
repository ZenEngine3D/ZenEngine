#pragma once
#ifndef __zCore_Res_Gfx_Binding_DX11_h__
#define __zCore_Res_Gfx_Binding_DX11_h__

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStripHAL_DX11 : public zcExp::ExportGfxMeshStrip
	{
	zenClassDeclare(GfxMeshStripHAL_DX11, zcExp::ExportGfxMeshStrip)
	public:
		GfxMeshStripHAL_DX11()
		{
			static int i(0);
			++i;
		}
		bool												Initialize();
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

		GfxIndexRef											mrIndexBuffer;
		GfxInputStreamRef									mrInputStream;			//!< Reference to input streams binding
		GfxShaderBindingRef									mrShaderBinding;
		zArrayStatic<GfxShaderParamRef>						marShaderParam;			//!< Array of all ShaderParam used by all Shaders stage
		zArrayStatic<zArrayStatic<GfxTexture2dRef>>			marTexture;				//!< Per Shader stage texture input (Array sub Index = Slot)
		zArrayStatic<zArrayStatic<GfxSamplerRef>>			marGfxSampler;			//!< Per Shader stage texture input (Array sub Index = Slot)

		typedef zcExp::ExporterGfxMeshStrip					RuntimeExporter;
	};
	class GfxMeshStripHAL : public GfxMeshStripHAL_DX11{};
	
	//=============================================================================================
	//! @class	Bind together all strip of a Mesh
	//=============================================================================================
	class GfxMeshHAL_DX11 : public zcExp::ExportGfxMesh
	{
	zenClassDeclare(GfxMeshHAL_DX11, zcExp::ExportGfxMesh)
	public:
		bool												Initialize();
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

		zArrayStatic<GfxMeshStripRef>						marGfxMeshStrip;		
		typedef zcExp::ExporterGfxMesh						RuntimeExporter;
	};
	class GfxMeshHAL : public GfxMeshHAL_DX11{};
	
	//=============================================================================================
	//! @class	Binding between a vertex shader and a VertexBuffer.
	//!			Needed for proper mapping of vertex stream into Shader.	
	//=============================================================================================
	class GfxInputStreamHAL_DX11 : public zcExp::ExportGfxInputStream
	{
	zenClassDeclare(GfxInputStreamHAL_DX11, zcExp::ExportGfxInputStream)
	public:
		virtual												~GfxInputStreamHAL_DX11();
		bool												Initialize();

		GfxVertexRef										mrVertexStream;
		GfxInputSignatureRef								mrSignature;
		ID3D11InputLayout*									mpInputLayout	= nullptr;	//!< Contain DX object for vertex input remapping
		typedef zcExp::ExporterGfxInputStream				RuntimeExporter;
	};
	class GfxInputStreamHAL : public GfxInputStreamHAL_DX11{};

	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBindingHAL_DX11 : public zcExp::ExportGfxShaderBinding
	{
	zenClassDeclare(GfxShaderBindingHAL_DX11, zcExp::ExportGfxShaderBinding)
	public:
		bool												Initialize();
		void												CreateShaderParam(zArrayStatic<zenRes::zGfxShaderParam>& _aShaderParamOut)const;

		GfxShaderAnyRef										marShader[zenConst::keShaderStage__Count];
		zMap<TextureSlot>::Key32							mdStageSlotPerTexture;	//!< Texture Index of each Shader stage using this parameter name
		zMap<zU32>::Key32									mdBufferPerParam;		//!< Bitmask of each ShaderParam containing this parameter name
		typedef zcExp::ExporterGfxShaderBindingDX11_DX11	RuntimeExporter;
	};
	class GfxShaderBindingHAL : public GfxShaderBindingHAL_DX11{};
	
}

#endif
