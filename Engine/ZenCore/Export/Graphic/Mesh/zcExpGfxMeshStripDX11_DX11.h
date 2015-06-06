#pragma once
#ifndef __zCore_Exp_Gfx_GfxMeshStrip_DX11_DX11_h__
#define __zCore_Exp_Gfx_GfxMeshStrip_DX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxMeshStripDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxMeshStripDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxMeshStripDX11		ExportData;
	typedef zGameRef<ExportData>			ExportDataRef;
											ExporterGfxMeshStripDX11_DX11(const ExportDataRef& _rExportData);

	protected:	
		virtual bool						ExportWork(bool _bIsTHRTask);
		virtual bool						ExportEnd();
		ExportDataRef						mrExportData;
	};
}

#endif
