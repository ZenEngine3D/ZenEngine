#pragma once
#ifndef __zCore_Exp_GfxShaderParamDX11_DX11_h__
#define __zCore_Exp_GfxShaderParamDX11_DX11_h__

namespace zcExp
{
	class ExporterGfxShaderParamDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderParamDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxShaderParamDX11	ResData;
	typedef zGameRef<ResData>			ResDataRef;
											ExporterGfxShaderParamDX11_DX11(const ResDataRef& _rResData);
	protected:	
		virtual bool						ExportStart();
		ResDataRef						mrResData;
	};
}

#endif
