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
	zenResID CreateGfxMeshStrip(zenResID _VertexBufferID, zenResID _IndexBufferID, zenResID _ShaderBindingID, zenU32 _uIndexFirst, zenU32 _uIndexCount, const zenArrayBase<zenResID>& _aShaderParamID, const zenArrayBase<zenRes::awShaderTexture>& _aTexture)
	{
		static zenMem::AllocatorPool sMemPool("Pool CreateMeshStrip", sizeof(SerialMeshStrip_Base::ExportInfo), 1, 5 );
		SerialMeshStrip_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialMeshStrip_Base::ExportInfo;
		pExportInfo->mVertexBufferID					= _VertexBufferID;	
		pExportInfo->mIndexBufferID						= _IndexBufferID;
		pExportInfo->mShaderBindingID					= _ShaderBindingID;
		pExportInfo->maTexture							= _aTexture;				
		pExportInfo->maShaderParamID					= _aShaderParamID;		
		pExportInfo->muIndexFirst						= _uIndexFirst;
		pExportInfo->muIndexCount						= _uIndexCount;
		return EMgr::Export.CreateItem( zenResID::kePlatformType_GFX, zenConst::keResType_GfxMeshStrip, pExportInfo );
	}


}

