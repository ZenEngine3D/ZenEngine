#pragma once
#ifndef __zCore_Exp_GfxMesh_DX11_DX11_h__
#define __zCore_Exp_GfxMesh_DX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxMeshDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxMeshDX11_DX11, ExporterBase)
	public:				
	typedef ExportDataGfxMeshDX11			ExportData;
	typedef zGameRef<ExportData>			ExportDataRef;
											ExporterGfxMeshDX11_DX11(const ExportDataRef& _rExportData);
	protected:	
		virtual bool						ExportStart();		
		ExportDataRef						mrExportData;
	};
}

#endif