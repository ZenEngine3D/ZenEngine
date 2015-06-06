#pragma once
#if !defined(__zCore_Exp_GfxMesh_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_GfxMesh_DX11_h__

namespace zcExp
{	
	class ExportDataGfxMeshDX11 : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxMeshDX11, ExportDataBase)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
	};
}

#endif