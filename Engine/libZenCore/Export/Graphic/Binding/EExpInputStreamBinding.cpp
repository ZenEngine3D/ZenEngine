#include "libZenCore.h"

namespace EExp
{
	//=================================================================================================
	//! @brief		Create a new ShaderInputSignature Binding to a VertexBuffer
	//! @details	Tie together a vertex stream and input signature, for rendering
	//-------------------------------------------------------------------------------------------------
	//! @param _VertexBufferID			- VertexBufer ID to bind with ShaderInputSignature
	//! @param _ShaderInputSignatureID	- ShaderInputSignature ID to bind with a particular VertexBuffer
	//! @return 						- Unique zenResID of created Resource
	//=================================================================================================
	zenResID CreateGfxInputStream(zenResID _VertexBufferID, zenResID _ShaderInputSignatureID)
	{
		static zenMem::AllocatorPool sMemPool("Pool CreateInputStream", sizeof(SerialInputStream_Base::ExportInfo), 1, 5 );
		SerialInputStream_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialInputStream_Base::ExportInfo;
		pExportInfo->mVertexBufferID							= _VertexBufferID;
		pExportInfo->mShaderInputSignatureID					= _ShaderInputSignatureID;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, zenConst::keResType_GfxInputStream, pExportInfo );
	}

}

