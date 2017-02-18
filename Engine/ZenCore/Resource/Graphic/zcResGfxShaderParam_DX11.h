#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_DX11_h__
#define __zCore_Res_Gfx_GfxShaderParam_DX11_h__

namespace zcRes
{
	//==============================================================================================
	// GfxCBufferDefinition_DX11 
	//==============================================================================================
	class GfxCBufferDefinition_DX11 : public zcExp::ExportGfxCBufferDefinition
	{
	zenClassDeclare(GfxCBufferDefinition_DX11, zcExp::ExportGfxCBufferDefinition)
	public:
		bool										Initialize();				
		typedef zcExp::ExporterGfxShaderParamDef	RuntimeExporter;
	};
	
	//==============================================================================================
	// GfxCBuffer_DX11
	//==============================================================================================
	class GfxCBuffer_DX11 : public zcExp::ExportGfxCBuffer
	{
	zenClassDeclare(GfxCBuffer_DX11, zcExp::ExportGfxCBuffer)
	public:
		virtual										~GfxCBuffer_DX11();
		bool										Initialize();
		void										Update( ID3D11DeviceContext& DirectXContext );
		void										SetValue(const zenRes::zShaderParameter& _Value);	
		void										SetValue(const zcExp::ParameterBase& _Value);	
		template<class TParamType> void				SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex=0);

		GfxCBufferDefinitionRef						mrCBufferParent;					//!< Parent Definition of this CBuffer
		ID3D11Buffer*								mpBufferBinding			= nullptr;	//!< DirectX object for buffer
		mutable bool								mbUpdated				= false;	//!< True when parameter value was changed and must update GPU buffer		
		typedef zcExp::ExporterGfxCBuffer			RuntimeExporter;
	};

	template<class TParamType>
	void GfxCBuffer_DX11::SetValue(zHash32 _hParamName, const TParamType& _Value, zU16 _uIndex)
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
