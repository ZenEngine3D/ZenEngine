#pragma once
#ifndef __LibExport_Graphics_Mesh_Base_h__
#define __LibExport_Graphics_Mesh_Base_h__

namespace EExp
{
	class SerialMesh_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialMesh_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			awArrayStatic<awResourceID>		maMeshStripID;			//!< List of Mesh strips used this Mesh
		};

		static awResourceID CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	awResourceID CreateGfxMesh(const awArrayBase<awResourceID>& _aMeshStripID);
}

#include "EExpMeshDX11.h"

#endif