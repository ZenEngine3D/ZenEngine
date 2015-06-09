#pragma once
#ifndef __zCore_Exp_GfxMesh_DX11_DX11_h__
#define __zCore_Exp_GfxMesh_DX11_DX11_h__

namespace zcExp
{	
	class ExporterGfxMeshDX11_DX11 : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxMeshDX11_DX11, ExporterBase)
	public:				
	typedef ResDataGfxMeshDX11			ResData;
	typedef zEngineRef<ResData>			ResDataRef;
										ExporterGfxMeshDX11_DX11(const ResDataRef& _rResData);
	protected:	
		virtual bool					ExportStart();		
		ResDataRef						mrResData;
	};
}

#endif