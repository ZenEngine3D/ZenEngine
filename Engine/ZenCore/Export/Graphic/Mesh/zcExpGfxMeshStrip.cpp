#include "zcCore.h"

namespace zcExp
{
	//=================================================================================================
	//! @brief		Create a new MeshStrip
	//! @details	Tie together vertex, index and shader, giving us all the info needed 
	//				to make a render call
	//-------------------------------------------------------------------------------------------------
	//! @param _VertexBufferID	- VertexBuffer Resource
	//! @param _IndexBufferID	- IndexBuffer Resource
	//! @param _ShaderBindingID	- Resource used for the shader config
	//! @param _uIndexFirst		- First index used in IndexBuffer, 
	//! @param _uIndexCount		- Number of Index to use in IndexBuffer
	//! @return 				- Created MeshStrip
	//=================================================================================================
	zResID CreateGfxMeshStrip(zResID _VertexBufferID, zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst, zU32 _uIndexCount, const zArrayBase<zResID>& _aShaderParamID, const zArrayBase<zenRes::zShaderTexture>& _aTexture)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateMeshStrip", sizeof(ExportInfoGfxMeshStrip), 1, 5 );
		ExportInfoGfxMeshStrip* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxMeshStrip;
		pExportInfo->mVertexBufferID			= _VertexBufferID;	
		pExportInfo->mIndexBufferID				= _IndexBufferID;
		pExportInfo->mShaderBindingID			= _ShaderBindingID;
		pExportInfo->maTexture					= _aTexture;				
		pExportInfo->maShaderParamID			= _aShaderParamID;		
		pExportInfo->muIndexFirst				= _uIndexFirst;
		pExportInfo->muIndexCount				= _uIndexCount;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxMeshStrip, pExportInfo );
	}


}

