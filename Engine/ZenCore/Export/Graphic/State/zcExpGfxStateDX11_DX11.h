#pragma once
#if !defined(__zCore_ExpGfxStateDX11_DX11_h__)
#define __zCore_ExpGfxStateDX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxSamplerDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxSamplerDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxSamplerDX11			ExportData;
	typedef zGameRef<ExportData>				ExportDataRef;
												ExporterGfxSamplerDX11_DX11(const ExportDataRef& _rExportData);		
		static zResID							CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportDataRef							mrExportData;
	};

	class ExporterGfxStateBlendDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateBlendDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxStateBlendDX11			ExportData;
	typedef zGameRef<ExportData>				ExportDataRef;
												ExporterGfxStateBlendDX11_DX11(const ExportDataRef& _rExportData);		
		static zResID							CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportDataRef							mrExportData;
	};

	class ExporterGfxStateDepthStencilDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateDepthStencilDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxStateDepthStencilDX11	ExportData;
	typedef zGameRef<ExportData>				ExportDataRef;
												ExporterGfxStateDepthStencilDX11_DX11(const ExportDataRef& _rExportData);		
		static zResID							CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportDataRef							mrExportData;
	};

	class ExporterGfxStateRasterizerDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxStateRasterizerDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxStateRasterizerDX11	ExportData;
	typedef zGameRef<ExportData>				ExportDataRef;
												ExporterGfxStateRasterizerDX11_DX11(const ExportDataRef& _rExportData);		
		static zResID							CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportDataRef							mrExportData;
	};
}

#endif