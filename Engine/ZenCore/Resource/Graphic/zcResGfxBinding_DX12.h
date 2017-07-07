#pragma once
//SF DX12

namespace zcRes
{
	//=============================================================================================
	//! @class	Binding geometry to shader
	//! @todo 2 Rename this to something more generic (to include compute shaders)
	//=============================================================================================
	class GfxMeshStrip_DX12 : public zcExp::ExportGfxMeshStrip
	{
	zenClassDeclare(GfxMeshStrip_DX12, zcExp::ExportGfxMeshStrip)
	public:
		bool										Initialize();
		void										SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0 );
		void										SetResource(zHash32 _hResourceName, const zArrayBase<GfxShaderResourceRef>& _arResources );
		void										SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void										SetValue(const zenRes::zShaderParameter& _Value);		
		template<class TParamType> void				SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);		

		GfxIndexRef									mrIndexBuffer;
		GfxShaderBindingRef							mrShaderBinding;												//!< Information on resources used between all shader stages
		zArrayStatic<GfxCBufferRef>					marConstantBuffer;												//!< Array of all ShaderParam used by all Shaders stage @todo refactor to work like other resources?
		zArrayStatic<GfxShaderResourceDescRef>		marShaderResources[keShaderStage__Count][keShaderRes__Count];	//!< List of all resources bound per 'shader stage'/'resource type'
		zHash32										mhShaderResourceStamp[keShaderStage__Count][keShaderRes__Count];//!< Hash of all resources bound per 'shader stage'/'resource type', used to quickly see if they're different than currently assign to GPU
		zArrayStatic<GfxShaderResourceDescRef>		marDescriptorResources[keShaderStage__Count];					//!< Resources associated in with each t#
		zArrayDynamic<GfxShaderResourceDescRef>		maTrackedResources;												//!< Resources to Keeps track of gpu state (to insert barrier when needed to transition) @todo 2 optim find better container heuristic
		
		typedef zcExp::ExporterGfxMeshStrip			RuntimeExporter;	
	protected:
		zenInline bool								HasResourceDescriptor(eShaderResource _eResType)const;
		zenInline void								TrackedResourceAdd(const GfxShaderResourceDescRef& _rResource);	//!< @brief Check if resource needs to be tracked, and add it if it is
		zenInline void								TrackedResourceRem(const GfxShaderResourceDescRef& _rResource);	//!< @brief Check if resource was tracked, and remove it from tracking if it was
	};
	
	bool GfxMeshStrip_DX12::HasResourceDescriptor(eShaderResource _eResType) const
	{
		return _eResType != keShaderRes_Sampler && _eResType != keShaderRes_Unknown;
	}

	template<class TParamType> 
	void GfxMeshStrip_DX12::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
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
	class GfxMesh_DX12 : public zcExp::ExportGfxMesh
	{
	zenClassDeclare(GfxMesh_DX12, zcExp::ExportGfxMesh)
	public:
		bool									Initialize();
		void									SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex=0);
		void									SetResource(zHash32 _hResourceName, const zArrayBase<GfxShaderResourceRef>& _arResources );
		void									SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);
		void									SetValue(const zenRes::zShaderParameter& _Value);
		template<class TParamType> void			SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		zArrayStatic<GfxMeshStripRef>			marGfxMeshStrip;		
		typedef zcExp::ExporterGfxMesh			RuntimeExporter;
	};
	
	template<class TParamType> void			
	GfxMesh_DX12::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
	{
		for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.Count()); stripIdx<stripCount; ++stripIdx)
			marGfxMeshStrip[stripIdx]->SetValue(_hParamName, _Value, _uIndex);
	}


	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBinding_DX12 : public zcExp::ExportGfxShaderBinding
	{
	zenClassDeclare(GfxShaderBinding_DX12, zcExp::ExportGfxShaderBinding)
	public:
		bool									Initialize();
		void									CreateShaderParam(zArrayStatic<zenRes::zGfxCBuffer>& Out_aShaderParam)const;

		GfxShaderAnyRef							marShader[keShaderStage__Count];
				

		typedef zcExp::ExporterGfxShaderBinding	RuntimeExporter;
	};	
}
