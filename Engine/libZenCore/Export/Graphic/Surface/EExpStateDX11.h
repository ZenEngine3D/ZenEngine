#pragma once
#if !defined(__LibExport_Graphics_SerialGfxSampler_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxSampler_DX11_h__

namespace EExp
{	
	class SerialGfxSampler_DX11 : public SerialSampler_Base
	{
	AWClassDeclare(SerialGfxSampler_DX11, SerialSampler_Base)
//-------------------------------------------------------------------------------------------------
// Serialization and Asset creation support
//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_SAMPLER_DESC mSamplerDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( EExp::Serializer_Base& _Serializer );	

//-------------------------------------------------------------------------------------------------
// Export
//-------------------------------------------------------------------------------------------------
		static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxBlend_DX11 : public SerialBlend_Base
	{
		AWClassDeclare(SerialGfxBlend_DX11, SerialBlend_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_BLEND_DESC mBlendDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( EExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxDepthStencil_DX11 : public SerialDepthStencil_Base
	{
		AWClassDeclare(SerialGfxDepthStencil_DX11, SerialDepthStencil_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_DEPTH_STENCIL_DESC mDepthStencilDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( EExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	

	class SerialGfxRasterizer_DX11 : public SerialRasterizer_Base
	{
		AWClassDeclare(SerialGfxRasterizer_DX11, SerialRasterizer_Base)
			//-------------------------------------------------------------------------------------------------
			// Serialization and Asset creation support
			//-------------------------------------------------------------------------------------------------
	public:		
		struct SerialUseOnly
		{
			D3D11_RASTERIZER_DESC mRasterizerDesc;
		};
		SerialUseOnly		mSerial;

		virtual bool		Serialize( EExp::Serializer_Base& _Serializer );	

		//-------------------------------------------------------------------------------------------------
		// Export
		//-------------------------------------------------------------------------------------------------
		static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};	
}

#endif