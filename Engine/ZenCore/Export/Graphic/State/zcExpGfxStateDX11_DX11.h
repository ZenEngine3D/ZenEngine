#pragma once
#if !defined(__zCore_ExpGfxStateDX11_DX11_h__)
#define __zCore_ExpGfxStateDX11_DX11_h__

namespace zcExp
{	

	class ExporterGfxStateSamplerDX11_DX11 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateSamplerDX11_DX11, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxStateSamplerDX11>	ExportResultRef; //todo use proper ref class
												ExporterGfxStateSamplerDX11_DX11(const ExportResultRef& _rExportOut);
		static zResID							CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportResultRef							mrExport;
	};

	class ExporterGfxStateBlendDX11_DX11 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateBlendDX11_DX11, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxStateBlendDX11>	ExportResultRef; //todo use proper ref class
												ExporterGfxStateBlendDX11_DX11(const ExportResultRef& _rExportOut);
		static zResID							CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool							ExportWork(bool _bIsTHRTask);	
		ExportResultRef							mrExport;
	};

	class ExporterGfxStateDepthStencilDX11_DX11 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateDepthStencilDX11_DX11, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxStateDepthStencilDX11>	ExportResultRef; //todo use proper ref class
														ExporterGfxStateDepthStencilDX11_DX11(const ExportResultRef& _rExportOut);
		static zResID									CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool									ExportWork(bool _bIsTHRTask);	
		ExportResultRef									mrExport;
	};

	class ExporterGfxStateRasterDX11_DX11 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateRasterDX11_DX11, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxStateRasterDX11>	ExportResultRef; //todo use proper ref class
													ExporterGfxStateRasterDX11_DX11(const ExportResultRef& _rExportOut);
		static zResID								CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool								ExportWork(bool _bIsTHRTask);	
		ExportResultRef								mrExport;
	};
}

#endif