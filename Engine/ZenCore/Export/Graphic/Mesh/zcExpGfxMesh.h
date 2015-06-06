#pragma once
#ifndef __zCore_Exp_Gfx_Mesh_Base_h__
#define __zCore_Exp_Gfx_Mesh_Base_h__

namespace zcExp
{	
	struct ExportInfoGfxMesh : public ExportInfoBase
	{
		zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	zResID CreateGfxMesh(const zArrayBase<zResID>& _aMeshStripID);
}

#include "zcExpGfxMeshDX11.h"

#endif