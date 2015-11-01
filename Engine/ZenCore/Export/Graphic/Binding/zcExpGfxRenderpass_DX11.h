#pragma once
#ifndef __zCore_Exp_Gfx_RenderPass_DX11_h__
#define __zCore_Exp_Gfx_RenderPass_DX11_h__

namespace zcExp
{ 	
	class ResDataGfxRenderpass_DX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxRenderpass_DX11, ResourceData)
	public:
		virtual bool					Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zString							mzStageName;
		zResID							mBlendStateID;
		zResID							mDepthStencilStateID;
		zResID							mRasterStateID;
		zResID							mViewStateID; 
		zUInt							muPassPriority;
	};

	class ExporterGfxRenderPass_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxRenderPass_DX11, ExporterBase)
	public:				
	typedef ResDataGfxRenderpass_DX11	ResData;
	typedef zEngineRef<ResData>			ResDataRef;
										ExporterGfxRenderPass_DX11(const ResDataRef& _rResData);
	protected:	
		virtual bool					ExportStart();		
		ResDataRef						mrResData;
	};
}

#endif
