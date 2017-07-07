#pragma once

namespace zcExp
{
	struct ExportInfoGfxView : public ExportInfoBase
	{
		const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>*	mpaRTColorConfig;
		const zenRes::zGfxRenderPass::ConfigDepthRT*				mpRTDepthConfig;
		zVec2U16													mvDim;
		zVec2S16													mvOrigin;
		static zResID												CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	class ExportGfxView : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxView, zenRes::zExportData)
	public:
		zArrayStatic<zenRes::zGfxRenderPass::ConfigColorRT>			maRTColorConfig;
		zenRes::zGfxRenderPass::ConfigDepthRT						mRTDepthConfig;
		zVec2U16													mvDim;
		zVec2S16													mvOrigin;
	};

	class ExporterGfxView : public ExporterBase
	{
	zenClassDeclare(ExporterGfxView, ExporterBase)
	public:
	typedef zEngineRef<ExportGfxView>	ExportResultRef; //todo use proper ref class
										ExporterGfxView(const ExportResultRef& _rExportOut);
	protected:
		virtual bool					ExportStart();
		ExportResultRef					mrExportResult;
	};

	zResID CreateGfxView(const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColorConfig, const zenRes::zGfxRenderPass::ConfigDepthRT& _RTDepthConfig, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));	
	
}
