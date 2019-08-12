#pragma once

namespace zcExp
{	
	class ExporterGfxShaderDX12_DX12 : public ExporterBase 
	{
	zenClassDeclare(ExporterGfxShaderDX12_DX12, ExporterBase)
	public:				
		typedef zEngineRef<ExportGfxShaderDX12>		ExportResultRef; //todo 3 use proper ref class
													ExporterGfxShaderDX12_DX12(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool								ExportWork(bool _bIsTHRTask);
		virtual bool								ExportEnd();
		bool										ExportWorkCompile();
		bool										ExportWorkExtractResources();
		
		ExportResultRef								mrExport;
		zArrayDyn< zStringHash32 >					maCBufferName;
		zArrayDyn< zArrayDyn<zHash32> >				maCBufferParamName;
		zArrayDyn< zArrayDyn<GfxCBufferParamInfo> >	maCBufferParamInfo;
		zArrayDyn< zArrayDyn<zU8> >					maCBufferDefaultValues;
	};
}
