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

	class ResDataGfxRenderTarget : public ResourceData
	{
	ZENClassDeclare(ResDataGfxRenderTarget, ResourceData)
	public:
		virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		IDXGISwapChain*						mpBackbuffer	= nullptr; //! @todo Change to windows res pointer
		zU8									muBackbufferId	= 0;
		zenConst::eTextureFormat			meFormat		= zenConst::keTexFormat_RGBA8;	
		zVec2U16							mvDim			= zVec2U16(1024,800);
		bool								mbSRGB			= true;
	};

	class ExporterGfxRenderTarget : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxRenderTarget, ExporterBase)
	public:				
	typedef ResDataGfxRenderTarget		ResData;
	typedef zEngineRef<ResData>			ResDataRef;
										ExporterGfxRenderTarget(const ResDataRef& _rResData);
	protected:	
		virtual bool					ExportStart();		
		ResDataRef						mrResData;
	};

	zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
}

#endif