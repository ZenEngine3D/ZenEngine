#include "libZenCore.h"

namespace EExp
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
	awResourceID CreateGfxMeshStrip(awResourceID _VertexBufferID, awResourceID _IndexBufferID, awResourceID _ShaderBindingID, awU32 _uIndexFirst, awU32 _uIndexCount, const awArrayBase<awResourceID>& _aShaderParamID, const awArrayBase<awres::awShaderTexture>& _aTexture)
	{
		static CMem::PoolAllocator sMemPool("Pool CreateMeshStrip", sizeof(SerialMeshStrip_Base::ExportInfo), 1, 5 );
		SerialMeshStrip_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialMeshStrip_Base::ExportInfo;
		pExportInfo->mVertexBufferID					= _VertexBufferID;	
		pExportInfo->mIndexBufferID						= _IndexBufferID;
		pExportInfo->mShaderBindingID					= _ShaderBindingID;
		pExportInfo->maTexture							= _aTexture;				
		pExportInfo->maShaderParamID					= _aShaderParamID;		
		pExportInfo->muIndexFirst						= _uIndexFirst;
		pExportInfo->muIndexCount						= _uIndexCount;
		return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxMeshStrip, pExportInfo );
	}


}

