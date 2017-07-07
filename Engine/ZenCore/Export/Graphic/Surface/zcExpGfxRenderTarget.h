#pragma once

namespace zcExp
{
	struct ExportInfoGfxRendertarget : public ExportInfoBase
	{
		zenConst::eTextureFormat	meFormat;	
		zVec2U16					mvDim;
		bool						mbSRGB;
	};

	class ExportGfxTarget2D : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxTarget2D, zenRes::zExportData)
	public:
		//! @todo move to texture resource?
		zResID						mTextureID;
		zenConst::eTextureFormat	meFormat		= zenConst::keTexFormat_RGBA8;	
		zVec2U16					mvDim			= zVec2U16(0,0);
		bool						mbSRGB			= true;
	};

	class ExporterGfxTarget2D : public ExporterBase
	{
	zenClassDeclare(ExporterGfxTarget2D, ExporterBase)
	public:				
		typedef zEngineRef<ExportGfxTarget2D>	ExportResultRef;
												ExporterGfxTarget2D(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool							ExportStart();		
		ExportResultRef							mrExport;
	};

	zResID CreateGfxRenderTarget(zenConst::eTextureFormat _eFormat, zVec2U16 _vDim, bool _bSrgb=true);
}
