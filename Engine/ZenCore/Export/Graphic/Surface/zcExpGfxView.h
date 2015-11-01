#pragma once
#ifndef __zCore_Exp_Gfx_View_Base_h__
#define __zCore_Exp_Gfx_View_Base_h__

namespace zcExp
{
	struct ExportInfoGfxView : public ExportInfoBase
	{
		const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>*	mpaRTColorConfig;
		const zenRes::zGfxRenderPass::ConfigDepthRT*				mpRTDepthConfig;
		zVec2U16													mvDim;
		zVec2S16													mvOrigin;
		static zResID												CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	class ResDataGfxView : public ResourceData
	{
	ZENClassDeclare(ResDataGfxView, ResourceData)
	public:		
		zArrayStatic<zenRes::zGfxRenderPass::ConfigColorRT>			maRTColorConfig;
		zenRes::zGfxRenderPass::ConfigDepthRT						mRTDepthConfig;
		zVec2U16													mvDim;
		zVec2S16													mvOrigin;
		virtual bool												Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	};

	class ExporterGfxView : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxView, ExporterBase)
	public:				
	typedef ResDataGfxView		ResData;
	typedef zEngineRef<ResData>	ResDataRef;
								ExporterGfxView(const ResDataRef& _rResData);
	protected:	
		virtual bool			ExportStart();		
		ResDataRef				mrResData;
	};

	zResID CreateGfxView(const zArrayBase<zenRes::zGfxRenderPass::ConfigColorRT>& _aRTColorConfig, const zenRes::zGfxRenderPass::ConfigDepthRT& _RTDepthConfig, const zVec2U16& _vDim = zVec2U16(0xFFFF, 0xFFFF), const zVec2S16& _vOrigin = zVec2S16(0,0));	
	
}


#endif