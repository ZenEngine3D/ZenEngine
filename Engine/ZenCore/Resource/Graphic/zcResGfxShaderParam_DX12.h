#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_DX12_h__
#define __zCore_Res_Gfx_GfxShaderParam_DX12_h__
//SF DX12

namespace zcRes
{
	//==============================================================================================
	// GfxCBufferDefinitionHAL_DX12 
	//==============================================================================================
	class GfxCBufferDefinitionHAL_DX12 : public zcExp::ExportGfxCBufferDefinition
	{
	zenClassDeclare(GfxCBufferDefinitionHAL_DX12, zcExp::ExportGfxCBufferDefinition)
	public:
		bool										Initialize();				
		typedef zcExp::ExporterGfxShaderParamDef	RuntimeExporter;
	};
	class GfxCBufferDefinitionHAL : public GfxCBufferDefinitionHAL_DX12{};
	
	//==============================================================================================
	// GfxCBufferHAL_DX12
	//==============================================================================================
	class GfxCBufferHAL_DX12 : public zcExp::ExportGfxCBuffer
	{
	zenClassDeclare(GfxCBufferHAL_DX12, zcExp::ExportGfxCBuffer)
	public:
		virtual										~GfxCBufferHAL_DX12();
		bool										Initialize();
		void										Update( ID3D11DeviceContext& DirectXContext );
		void										SetValue(const zenRes::zShaderParameter& _Value);	
		void										SetValue(const zcExp::ParameterBase& _Value);	
		template<class TParamType> void				SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		GfxCBufferDefinitionRef						mrCBufferParent;					//!< Parent Definition of this CBuffer
		DirectXComRef<ID3D12Resource>				mrBuffer;
		DirectXComRef<ID3D12Resource>				mrLockData;
		D3D12_RESOURCE_STATES						meBufferState;		
		zcGfx::DescriptorSRV_UAV_CBV				mCBufferView;
		zU8*										mpCBufferMapped			= nullptr;	//!< CPU address of constant buffer we can write to
		mutable bool								mbUpdated				= false;	//!< True when parameter value was changed and must update GPU buffer		
		typedef zcExp::ExporterGfxCBuffer			RuntimeExporter;
	};
	class GfxCBufferHAL : public GfxCBufferHAL_DX12{};


	template<class TParamType>
	void GfxCBufferHAL_DX12::SetValue(const zHash32& _hParamName, const TParamType& _Value, zU16 _uIndex)
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
