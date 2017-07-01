#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_DX12_h__
#define __zCore_Res_Gfx_GfxShaderParam_DX12_h__
//SF DX12

namespace zcRes
{
	//==============================================================================================
	// GfxCBufferDefinition_DX12 
	//==============================================================================================
	class GfxCBufferDefinition_DX12 : public zcExp::ExportGfxCBufferDefinition
	{
	zenClassDeclare(GfxCBufferDefinition_DX12, zcExp::ExportGfxCBufferDefinition)
	public:
		bool										Initialize();				
		typedef zcExp::ExporterGfxShaderParamDef	RuntimeExporter;
	};
	
	//==============================================================================================
	// GfxCBuffer_DX12
	//==============================================================================================
	class GfxCBuffer_DX12 : public zcExp::ExportGfxCBuffer
	{
	zenClassDeclare(GfxCBuffer_DX12, zcExp::ExportGfxCBuffer)
	public:
		virtual										~GfxCBuffer_DX12();
		bool										Initialize();
		void										Update( const DirectXComRef<ID3D12GraphicsCommandList>& _rCommandList );
		void										SetValue(const zenRes::zShaderParameter& _Value);	
		void										SetValue(const zcExp::ParameterBase& _Value);	
		template<class TParamType> void				SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		DirectXComRef<ID3D12Resource>				mrResource;			//!< DirectX memory mapping for this resource
		DirectXComRef<ID3D12Resource>				mrResourceUpload;	//!< DirectX memory used to update GPU data from CPU
		D3D12_RESOURCE_STATES						meResourceState;	//!< Current GPU access to this resource (used for barrier updates)	
		zcGfx::DescriptorRangeSRV					mCBufferView;
		zU8*										mpCBufferMapped			= nullptr;	//!< CPU address of constant buffer we can write to
		GfxCBufferDefinitionRef						mrCBufferParent;					//!< Parent Definition of this CBuffer		
		mutable bool								mbUpdated				= false;	//!< True when parameter value was changed and must update GPU buffer		
		typedef zcExp::ExporterGfxCBuffer			RuntimeExporter;
	};

	template<class TParamType>
	void GfxCBuffer_DX12::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
	{
		zcExp::GfxCBufferParamInfo ItemInfo;
		if( mrCBufferParent.HAL()->mdParamInfo.Get(_hParamName, ItemInfo) )
		{			
			zenAssertMsg(ItemInfo.muSize == sizeof(TParamType), "Shader parameter is not the same size than provided templated type, type must match" );
			TParamType* pValues	= reinterpret_cast<TParamType*>(&maParameterValues[ItemInfo.muOffset]);
			zU8 uIndex			= static_cast<zU8>(zenMath::Min<zU16>(_uIndex, ItemInfo.muArrayCount));
			//zenAssertMsg(uIndex==_uIndex, "Invalid array index"); //!todo warning Error Message
			pValues[uIndex]		= _Value;		
			mbUpdated			= TRUE;
		}	
	}
}

#endif
