#pragma once
#if !defined(__zCore_Exp_GfxMesh_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_GfxMesh_DX11_h__

namespace zcExp
{	
	//! @todo clean move this to paltform generic entry, no need for class specialization
	class ResDataGfxMeshDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxMeshDX11, ResourceData)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
	};
}

#endif