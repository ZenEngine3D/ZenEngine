#pragma once
#ifndef __zCore_Exp_Gfx_RenderPass_Base_h__
#define __zCore_Exp_Gfx_RenderPass_Base_h__

namespace zcExp
{
	/*
	struct ExportInfoGfxView : public ExportInfoBase
	{
		zArrayStatic<zResID>	maTargetColorID;
		zResID					mTargetDepthID;
		zVec2S16				mvOrigin;
		zVec2U16				mvDim;
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	*/

	struct ExportInfoGfxRenderPass : public ExportInfoBase
	{
		zString						mzStageName;
		zResID						mBlendStateID;
		zResID						mDepthStencilStateID;
		zResID						mRasterStateID;
		zResID						mViewStateID;
	};
	
	//! @todo Urgent, Collapse view into this object, remove views
	class ResDataGfxRenderPass : public ResourceData
	{
	ZENClassDeclare(ResDataGfxRenderPass, ResourceData)
	public:
		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zString						mzStageName;
		zResID						mBlendStateID;
		zResID						mDepthStencilStateID;
		zResID						mRasterStateID;
		zResID						mViewStateID;
	};

	class ExporterGfxRenderPass : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxRenderPass, ExporterBase)
	public:				
	typedef ResDataGfxRenderPass	ResData;
	typedef zEngineRef<ResData>		ResDataRef;
									ExporterGfxRenderPass(const ResDataRef& _rResData);
	protected:	
		virtual bool				ExportStart();		
		ResDataRef					mrResData;
	};

	zResID CreateGfxRenderPass(const zString& _zStageName, zResID _BlendStateID, zResID _DepthStencilStateID, zResID _RasterStateID, zResID _ViewStateID);
}

#endif
