#include "zcCore.h"

namespace zcExp
{
	//=================================================================================================
	//! @brief		Create a new ShaderInputSignature Binding to a VertexBuffer
	//! @details	Tie together a vertex stream and input signature, for rendering
	//-------------------------------------------------------------------------------------------------
	//! @param _VertexBufferID			- VertexBufer ID to bind with ShaderInputSignature
	//! @param _ShaderInputSignatureID	- ShaderInputSignature ID to bind with a particular VertexBuffer
	//! @return 						- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateInputStream", sizeof(ExportInfoGfxInputStream), 1, 5 );
		ExportInfoGfxInputStream* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxInputStream;
		pExportInfo->mVertexBufferID			= _VertexBufferID;
		pExportInfo->mShaderInputSignatureID	= _ShaderInputSignatureID;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxInputStream, pExportInfo );
	}

}

