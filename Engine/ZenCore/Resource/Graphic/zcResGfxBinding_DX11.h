#pragma once

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStrip_DX11 : public zcExp::ExportGfxMeshStrip
	{
	zenClassDeclare(GfxMeshStrip_DX11, zcExp::ExportGfxMeshStrip)
	public:
		bool									Initialize();
		void									SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0 );
		void									SetResource(zHash32 _hResourceName, const zArray<GfxShaderResourceRef>& _arResources );
		void									SetValue(const zArray<const zenRes::zShaderParameter*>& _aValues);
		void									SetValue(const zenRes::zShaderParameter& _Value);		
		template<class TParamType> void			SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		GfxIndexRef								mrIndexBuffer;
		GfxShaderBindingRef						mrShaderBinding;													//!< Information on resources used between all shader stages
		zArrayDyn<GfxCBufferRef>				marConstantBuffer;													//!< Array of all ShaderParam used by all Shaders stage @todo refactor to work like other resources?
		zArrayDyn<GfxShaderResourceRef>			marShaderResources[keShaderStage__Count][keShaderRes__Count];		//!< List of all resources bound per 'shader stage'/'resource type'
		zHash32									mhShaderResourceStamp[keShaderStage__Count][keShaderRes__Count];	//!< Hash of all resources bound per 'shader stage'/'resource type', used to quickly see if they're different than currently assign to GPU
		
		typedef zcExp::ExporterGfxMeshStrip		RuntimeExporter;
	};
	
	template<class TParamType> 
	void GfxMeshStrip_DX11::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
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
	class GfxMesh_DX11 : public zcExp::ExportGfxMesh
	{
	zenClassDeclare(GfxMesh_DX11, zcExp::ExportGfxMesh)
	public:
		bool								Initialize();
		void								SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0);
		void								SetResource(zHash32 _hResourceName, const zArray<GfxShaderResourceRef>& _arResources );
		void								SetValue(const zArray<const zenRes::zShaderParameter*>& _aValues);
		void								SetValue(const zenRes::zShaderParameter& _Value);
		template<class TParamType> void		SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		zArrayDyn<GfxMeshStripRef>			marGfxMeshStrip;		
		typedef zcExp::ExporterGfxMesh		RuntimeExporter;
	};
	
	template<class TParamType> void			
	GfxMesh_DX11::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
	{
		for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
			marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _Value, _uIndex);
	}


	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBinding_DX11 : public zcExp::ExportGfxShaderBinding
	{
	zenClassDeclare(GfxShaderBinding_DX11, zcExp::ExportGfxShaderBinding)
	public:
		bool									Initialize();
		void									CreateShaderParam(zArrayDyn<zenRes::zGfxCBuffer>& Out_aShaderParam)const;

		GfxShaderAnyRef							marShader[keShaderStage__Count];
		typedef zcExp::ExporterGfxShaderBinding	RuntimeExporter;
	};	
}
