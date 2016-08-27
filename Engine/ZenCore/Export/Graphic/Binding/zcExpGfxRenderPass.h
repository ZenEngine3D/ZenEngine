#pragma once
#ifndef __zCore_Exp_Gfx_RenderPass_Base_h__
#define __zCore_Exp_Gfx_RenderPass_Base_h__

namespace zcExp
{ 
	struct ExportInfoGfxRenderpass : public ExportInfoBase
	{
		zString														mzStageName;
		zResID														mRasterStateID;	
		zUInt														muPassPriority;
		const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>*	mpaConfigRTColor;
		const zenRes::zGfxRenderPass::ConfigDepthRT*				mpConfigRTDepth;
		zenRes::zGfxStateRaster										mrStateRaster;
		zVec2U16													mvDim;
		zVec2S16													mvOrigin;
	};

	class ExportGfxRenderPass : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxRenderPass, zenRes::zExportData)
	public:
		//virtual bool					Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zString							mzStageName;
		zResID							mBlendStateID;
		zResID							mDepthStencilStateID;
		zResID							mRasterStateID;
		zResID							mViewStateID; 
		zUInt							muPassPriority;
	};

	class ExporterGfxRenderPass : public ExporterBase
	{
	zenClassDeclare(ExporterGfxRenderPass, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxRenderPass>		ExportResultRef; //todo use proper ref class
												ExporterGfxRenderPass(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool							ExportStart();		
		ExportResultRef							mrExport;
	};

	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zenRes::zGfxRenderPass::ConfigColorRT& _ConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRaster _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
	zResID CreateGfxRenderpass(const zString& _zStageName, zUInt _uPassPriority, const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aConfigRTColor, const zenRes::zGfxRenderPass::ConfigDepthRT& _ConfigRTDepth, zenRes::zGfxStateRaster _rStateRaster, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));
}

#endif
