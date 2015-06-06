#pragma once
#ifndef __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDef_DX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDefDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderParamDefDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxShaderParamDefDX11	ResData;
	typedef zGameRef<ResData>			ResDataRef;
											ExporterGfxShaderParamDefDX11_DX11(const ResDataRef& _rResData);
	protected:	
		virtual bool						ExportStart();
		virtual bool						ExportWork(bool _bIsTHRTask);	
		ResDataRef						mrResData;
		zArrayStatic<zU8>					maCompiledShader;
	};
}

#endif
