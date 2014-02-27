#include "libZenCore.h"

namespace EMgr { ERes::ManagerResource Resources; }

namespace ERes
{
//=================================================================================================
//! @brief		Create new dynamicly create resources , at runtime.
//! @details	
//! @param		_eResType
//! @param		_pExportInfo
//! @return 	
//=================================================================================================
zenResID CreateExportItemRuntime(EExp::ExportInfoBase* _pExportInfo)
{
	ZENAssert(_pExportInfo);
	awres::awResourceRef rResource;
	switch( _pExportInfo->mExportResID.Type() )
	{
	case awconst::keResType_GfxShaderPixel:				rResource = GfxShaderPixel::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxShaderVertex:			rResource = GfxShaderVertex::Create(*_pExportInfo);			break;	
	case awconst::keResType_GfxShaderParamDef:			rResource = GfxShaderParamDef::Create(*_pExportInfo);		break;
	case awconst::keResType_GfxShaderParam:				rResource = GfxShaderParam::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxShaderBinding:			rResource = GfxShaderBinding::Create(*_pExportInfo);		break;
	case awconst::keResType_GfxIndex:					rResource = GfxIndex::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxVertex:					rResource = GfxVertex::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxTexture2D:				rResource = GfxTexture2D::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxRenderTarget:			rResource = GfxRenderTarget::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxView:					rResource = GfxView::Create(*_pExportInfo);					break;
	case awconst::keResType_GfxWindow:					rResource = GfxWindow::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxMeshStrip:				rResource = GfxMeshStrip::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxMesh:					rResource = GfxMesh::Create(*_pExportInfo);					break;
	case awconst::keResType_GfxInputStream:				rResource = GfxInputStream::Create(*_pExportInfo);			break;
	case awconst::keResType_GfxInputSignature:			rResource = GfxInputSignature::Create(*_pExportInfo);		break;
	case awconst::keResType_GfxSampler:					rResource = GfxSampler::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxBlend:					rResource = GfxBlend::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxDepthStencil:			rResource = GfxDepthStencil::Create(*_pExportInfo);				break;
	case awconst::keResType_GfxRasterizer:				rResource = GfxRasterizer::Create(*_pExportInfo);				break;
	}
	zenDelNull(_pExportInfo);
	return rResource.GetResID();
}

//=================================================================================================
//! @brief		Called at engine start
//! @return 	True if no error occurred
//! @todo		Properly evaluate pre-allocation / growth policy
//=================================================================================================
bool ManagerResource::Load()
{		
	mdResources.Init(1024);	
	zenMem::Set(maResourcesDefault, sizeof(maResourcesDefault), 0);
	
	awconst::eResPlatform aPlatformTypes[zenResID::kePlatformType__Count];
	aPlatformTypes[zenResID::kePlatformType_OS]		= awconst::kAWCurrentPlatformOS;
	aPlatformTypes[zenResID::kePlatformType_GFX]	= awconst::kAWCurrentPlatformGfx;
	EMgr::Export.SetExportInfos( aPlatformTypes, awconst::keResSource_Runtime, CreateExportItemRuntime );
	return true;
}

//=================================================================================================
//! @brief		Called at engine end to free up resources
//! @return 	True if no error occurred
//! @todo		Unload resources
//=================================================================================================
bool ManagerResource::Unload()
{
	return true;
}

//=================================================================================================
//! @brief		Find and return the resource with a particular zenResID
//! @details	
//! @param		_ResID				- zenResID to look for
//! @param		_bSupportDefault	- If returning default resource of this type, when not found
//! @return 	Reference to resource
//=================================================================================================
awres::awResourceRef ManagerResource::GetResource(const zenResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());cd
	awres::awResource* pResource;
	if( mdResources.Get(_ResID.HashID(), pResource) )	return pResource;
	else if(_bSupportDefault )							return maResourcesDefault[_ResID.Type()];
	else												return (awres::awResource*)NULL;
}

//=================================================================================================
//! @brief		Find and return the resource with a particular zenResID
//! @details	This version try to look in both Loaded and Dynamic created resource.
//!				Useful for resource that can be dynamic and doesn't use an index for the name
//! @param		_ResID				- zenResID to look for
//! @param		_bSupportDefault	- If returning default resource of this type, when not found
//! @return 	Reference to resource
//=================================================================================================
awres::awResourceRef ManagerResource::GetResourceAnySource(const zenResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());
	awres::awResource* pResource;
	zenResID anySourceResID(_ResID);
	anySourceResID.SetSource(awconst::keResSource_Loaded);
	if( mdResources.Get(_ResID.HashID(), pResource) )	
	{
		return pResource;
	}
	else
	{
		anySourceResID.SetSource(awconst::keResSource_Runtime);	
		if( mdResources.Get(_ResID.HashID(), pResource) )	return pResource;
		else if(_bSupportDefault )							return maResourcesDefault[_ResID.Type()];
		else												return (awres::awResource*)NULL;
	}
}

//=================================================================================================
//! @brief		Add a resource to our resource dictionary
//! @details	This manager takes ownership of the resource and will free it once uneeded
//! @param		_pResource - Resource asset
//! @return 	Reference to resource
//=================================================================================================
awres::awResourceRef ManagerResource::Add(awres::awResource* _pResource)
{
	if( _pResource && _pResource->IsValid() )	
	{
		mdResources.Set(_pResource->GetResID().HashID(), _pResource);
		return _pResource;
	}
	return (awres::awResource*)NULL;
}
}