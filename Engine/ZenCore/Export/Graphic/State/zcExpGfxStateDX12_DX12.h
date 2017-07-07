#pragma once
#if !defined(__zCore_ExpGfxStateDX12_DX12_h__)
#define __zCore_ExpGfxStateDX12_DX12_h__

namespace zcExp
{	

	class ExporterGfxStateSamplerDX12_DX12 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateSamplerDX12_DX12, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxStateSamplerDX12>		ExportResultRef; //todo use proper ref class
														ExporterGfxStateSamplerDX12_DX12(const ExportResultRef& _rExportOut);
		static zResID									CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool									ExportWork(bool _bIsTHRTask);	
		ExportResultRef									mrExport;
	};

	class ExporterGfxStateBlendDX12_DX12 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateBlendDX12_DX12, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxStateBlendDX12>			ExportResultRef; //todo use proper ref class
														ExporterGfxStateBlendDX12_DX12(const ExportResultRef& _rExportOut);
		static zResID									CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool									ExportWork(bool _bIsTHRTask);	
		ExportResultRef									mrExport;
	};

	class ExporterGfxStateDepthStencilDX12_DX12 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateDepthStencilDX12_DX12, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxStateDepthStencilDX12>	ExportResultRef; //todo use proper ref class
														ExporterGfxStateDepthStencilDX12_DX12(const ExportResultRef& _rExportOut);
		static zResID									CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool									ExportWork(bool _bIsTHRTask);	
		ExportResultRef									mrExport;
	};

	class ExporterGfxStateRasterDX12_DX12 : public ExporterBase
	{
	zenClassDeclare(ExporterGfxStateRasterDX12_DX12, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxStateRasterDX12>		ExportResultRef; //todo use proper ref class
														ExporterGfxStateRasterDX12_DX12(const ExportResultRef& _rExportOut);
		static zResID									CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool									ExportWork(bool _bIsTHRTask);	
		ExportResultRef									mrExport;
	};
}

#endif