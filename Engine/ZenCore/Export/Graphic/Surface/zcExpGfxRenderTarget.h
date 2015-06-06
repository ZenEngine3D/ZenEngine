#pragma once
#ifndef __zCore_Exp_Gfx_Surface_RenderTarget_h__
#define __zCore_Exp_Gfx_Surface_RenderTarget_h__

namespace zcExp
{
	struct ExportInfoGfxRendertarget : public ExportInfoBase
	{
		zenConst::eTextureFormat			meFormat;	
		zVec2U16							mvDim;
		bool								mbSRGB;
	};

	class ResDataGfxRenderTarget : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxRenderTarget, ResDataBase)
	public:
											ResDataGfxRenderTarget(){mpBackbuffer=NULL;}
		virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		IDXGISwapChain*						mpBackbuffer; //! @todo Change to windows res pointer
		zenConst::eTextureFormat			meFormat;	
		zVec2U16							mvDim;
		bool								mbSRGB;
	};

	class ExporterGfxRenderTarget : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxRenderTarget, ExporterBase)
	public:				
	typedef ResDataGfxRenderTarget		ResData;
	typedef zGameRef<ResData>			ResDataRef;
											ExporterGfxRenderTarget(const ResDataRef& _rResData);
	protected:	
		virtual bool						ExportStart();		
		ResDataRef						mrResData;
	};

	zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
}

#endif