#pragma once
#ifndef __zCore_Res_Gfx_Binding_DX12_h__
#define __zCore_Res_Gfx_Binding_DX12_h__
//SF DX12

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStripHAL_DX12 : public zcExp::ExportGfxMeshStrip
	{
	zenClassDeclare(GfxMeshStripHAL_DX12, zcExp::ExportGfxMeshStrip)
	public:
		bool									Initialize();
		void									SetResource(const zHash32& _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0 );
		void									SetResource(const zHash32& _hResourceName, const zArrayBase<GfxShaderResourceRef>& _arResources );
		void									SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void									SetValue(const zenRes::zShaderParameter& _Value);		
		template<class TParamType> void			SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		GfxIndexRef								mrIndexBuffer;
		GfxShaderBindingRef						mrShaderBinding;													//!< Information on resources used between all shader stages
		zArrayStatic<GfxCBufferRef>				marConstantBuffer;													//!< Array of all ShaderParam used by all Shaders stage @todo refactor to work like other resources?
		zArrayStatic<GfxShaderResourceRef>		marShaderResources[keShaderStage__Count][keShaderRes__Count];		//!< List of all resources bound per 'shader stage'/'resource type'
		zHash32									mhShaderResourceStamp[keShaderStage__Count][keShaderRes__Count];	//!< Hash of all resources bound per 'shader stage'/'resource type', used to quickly see if they're different than currently assign to GPU
		
		typedef zcExp::ExporterGfxMeshStrip		RuntimeExporter;
	};
	class GfxMeshStripHAL : public GfxMeshStripHAL_DX12{};
	
	template<class TParamType> 
	void GfxMeshStripHAL_DX12::SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex)
	{
		auto paramMask= mrShaderBinding.HAL()->mdCBufferParamMask[_hParamName];
		while( paramMask != 0 )
		{
			zUInt cbufferIdx = zenMath::BitsScan( paramMask );
			paramMask		^= (1<<cbufferIdx);
			marConstantBuffer[cbufferIdx]->SetValue(_hParamName, _Value, _uIndex);
		}
	}

	//=============================================================================================
	//! @class	Bind together all strip of a Mesh
	//=============================================================================================
	class GfxMeshHAL_DX12 : public zcExp::ExportGfxMesh
	{
	zenClassDeclare(GfxMeshHAL_DX12, zcExp::ExportGfxMesh)
	public:
		bool									Initialize();
		void									SetResource(const zHash32& _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0);
		void									SetResource(const zHash32& _hResourceName, const zArrayBase<GfxShaderResourceRef>& _arResources );
		void									SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void									SetValue(const zenRes::zShaderParameter& _Value);
		template<class TParamType> void			SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		zArrayStatic<GfxMeshStripRef>			marGfxMeshStrip;		
		typedef zcExp::ExporterGfxMesh			RuntimeExporter;
	};
	class GfxMeshHAL : public GfxMeshHAL_DX12{};
	
	template<class TParamType> void			
	GfxMeshHAL_DX12::SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex)
	{
		for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
			marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _Value, _uIndex);
	}


	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBindingHAL_DX12 : public zcExp::ExportGfxShaderBinding
	{
	zenClassDeclare(GfxShaderBindingHAL_DX12, zcExp::ExportGfxShaderBinding)
	public:
		bool									Initialize();
		void									CreateShaderParam(zArrayStatic<zenRes::zGfxCBuffer>& Out_aShaderParam)const;

		GfxShaderAnyRef							marShader[keShaderStage__Count];
		typedef zcExp::ExporterGfxShaderBinding	RuntimeExporter;
	};
	class GfxShaderBindingHAL : public GfxShaderBindingHAL_DX12{};
	
}

#endif
