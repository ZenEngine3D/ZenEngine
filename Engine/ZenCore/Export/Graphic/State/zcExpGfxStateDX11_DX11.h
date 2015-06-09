#pragma once
#if !defined(__zCore_ExpGfxStateDX11_DX11_h__)
#define __zCore_ExpGfxStateDX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxSamplerDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxSamplerDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxSamplerDX11			ResData;
	typedef zEngineRef<ResData>				ResDataRef;
											ExporterGfxSamplerDX11_DX11(const ResDataRef& _rResData);		
		static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ResDataRef							mrResData;
	};

	class ExporterGfxStateBlendDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateBlendDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxStateBlendDX11		ResData;
	typedef zEngineRef<ResData>				ResDataRef;
											ExporterGfxStateBlendDX11_DX11(const ResDataRef& _rResData);		
		static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ResDataRef							mrResData;
	};

	class ExporterGfxStateDepthStencilDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateDepthStencilDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxStateDepthStencilDX11	ResData;
	typedef zEngineRef<ResData>				ResDataRef;
											ExporterGfxStateDepthStencilDX11_DX11(const ResDataRef& _rResData);		
		static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ResDataRef							mrResData;
	};

	class ExporterGfxStateRasterizerDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateRasterizerDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxStateRasterizerDX11	ResData;
	typedef zEngineRef<ResData>				ResDataRef;
											ExporterGfxStateRasterizerDX11_DX11(const ResDataRef& _rResData);		
		static zResID						CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ResDataRef							mrResData;
	};
}

#endif