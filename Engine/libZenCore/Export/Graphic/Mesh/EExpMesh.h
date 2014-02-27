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
			zenArrayStatic<zenResID>		maMeshStripID;			//!< List of Mesh strips used this Mesh
		};

		static zenResID CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	zenResID CreateGfxMesh(const zenArrayBase<zenResID>& _aMeshStripID);
}

#include "EExpMeshDX11.h"

#endif