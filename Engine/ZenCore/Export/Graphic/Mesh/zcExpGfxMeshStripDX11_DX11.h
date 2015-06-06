#pragma once
#ifndef __zCore_Exp_Gfx_GfxMeshStrip_DX11_DX11_h__
#define __zCore_Exp_Gfx_GfxMeshStrip_DX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxMeshStripDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxMeshStripDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxMeshStripDX11		ResData;
	typedef zGameRef<ResData>			ResDataRef;
											ExporterGfxMeshStripDX11_DX11(const ResDataRef& _rResData);

	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);
		virtual bool						ExportEnd();
		ResDataRef						mrResData;
	};
}

#endif
