#pragma once
#ifndef __LibExport_Graphics_Mesh_Base_h__
#define __LibExport_Graphics_Mesh_Base_h__

namespace EExp
{
	class SerialMesh_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialMesh_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zArrayStatic<zResID>		maMeshStripID;			//!< List of Mesh strips used this Mesh
		};

		static zResID CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	zResID CreateGfxMesh(const zArrayBase<zResID>& _aMeshStripID);
}

#include "EExpMeshDX11.h"

#endif