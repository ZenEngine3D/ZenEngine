#pragma once

namespace zcExp
{	
	class ExporterGfxShaderDX11_DX11 : public ExporterBase 
	{
	zenClassDeclare(ExporterGfxShaderDX11_DX11, ExporterBase)
	public:				
		typedef zEngineRef<ExportGfxShaderDX11>				ExportResultRef; //todo use proper ref class
															ExporterGfxShaderDX11_DX11(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool										ExportWork(bool _bIsTHRTask);
		virtual bool										ExportEnd();
		bool												ExportWorkCompile();
		bool												ExportWorkExtractResources();
		
		ExportResultRef										mrExport;
		zArrayStatic< zStringHash32 >						maCBufferName;
		zArrayStatic< zArrayStatic<zHash32> >				maCBufferParamName;
		zArrayStatic< zArrayStatic<GfxCBufferParamInfo> >	maCBufferParamInfo;
		zArrayStatic< zArrayStatic<zU8> >					maCBufferDefaultValues;
	};
}
