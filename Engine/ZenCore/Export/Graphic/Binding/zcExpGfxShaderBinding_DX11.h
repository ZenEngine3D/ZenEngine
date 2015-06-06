#pragma once
#ifndef __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__
#define __zCore_Exp_Gfx_GfxShaderBinding_DX11_h__

namespace zcExp
{	
	class ExporterGfxShaderBindingDX11_DX11 : public ExporterGfxShaderBinding
	{
	ZENClassDeclare(ExporterGfxShaderBindingDX11_DX11, ExporterGfxShaderBinding)
	public:				
	typedef ResDataGfxShaderBinding		ResData;
	typedef zGameRef<ResData>			ResDataRef;
											ExporterGfxShaderBindingDX11_DX11(const ResDataRef& _rResData);
	protected:	
		virtual bool						ExportStart();		
		ResDataRef						mrResData;
	};
}

#endif