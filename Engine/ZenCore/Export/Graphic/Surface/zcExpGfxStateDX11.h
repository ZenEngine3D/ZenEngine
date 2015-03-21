#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxSampler_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxSampler_DX11_h__

namespace zcExp
{	
	class SerialGfxSampler_DX11 : public SerialSampler_Base
	{
	ZENClassDeclare(SerialGfxSampler_DX11, SerialSampler_Base)
//-------------------------------------------------------------------------------------------------
// Serialization and Asset creation support
//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_SAMPLER_DESC mSamplerDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( zcExp::Serializer_Base& _Serializer );	

//-------------------------------------------------------------------------------------------------
// Export
//-------------------------------------------------------------------------------------------------
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxBlend_DX11 : public SerialBlend_Base
	{
		ZENClassDeclare(SerialGfxBlend_DX11, SerialBlend_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_BLEND_DESC mBlendDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( zcExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxDepthStencil_DX11 : public SerialDepthStencil_Base
	{
		ZENClassDeclare(SerialGfxDepthStencil_DX11, SerialDepthStencil_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( zcExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxRasterizer_DX11 : public SerialRasterizer_Base
	{
		ZENClassDeclare(SerialGfxRasterizer_DX11, SerialRasterizer_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_RASTERIZER_DESC mRasterizerDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( zcExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	
}

#endif