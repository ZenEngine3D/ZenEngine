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
	zenRes::awResourceRef rResource;
	switch( _pExportInfo->mExportResID.Type() )
	{
	case zenConst::keResType_GfxShaderPixel:				rResource = GfxShaderPixel::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxShaderVertex:			rResource = GfxShaderVertex::Create(*_pExportInfo);			break;	
	case zenConst::keResType_GfxShaderParamDef:			rResource = GfxShaderParamDef::Create(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderParam:				rResource = GfxShaderParam::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxShaderBinding:			rResource = GfxShaderBinding::Create(*_pExportInfo);		break;
	case zenConst::keResType_GfxIndex:					rResource = GfxIndex::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxVertex:					rResource = GfxVertex::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxTexture2D:				rResource = GfxTexture2D::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxRenderTarget:			rResource = GfxRenderTarget::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxView:					rResource = GfxView::Create(*_pExportInfo);					break;
	case zenConst::keResType_GfxWindow:					rResource = GfxWindow::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxMeshStrip:				rResource = GfxMeshStrip::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxMesh:					rResource = GfxMesh::Create(*_pExportInfo);					break;
	case zenConst::keResType_GfxInputStream:				rResource = GfxInputStream::Create(*_pExportInfo);			break;
	case zenConst::keResType_GfxInputSignature:			rResource = GfxInputSignature::Create(*_pExportInfo);		break;
	case zenConst::keResType_GfxSampler:					rResource = GfxSampler::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxBlend:					rResource = GfxBlend::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxDepthStencil:			rResource = GfxDepthStencil::Create(*_pExportInfo);				break;
	case zenConst::keResType_GfxRasterizer:				rResource = GfxRasterizer::Create(*_pExportInfo);				break;
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
	
	zenConst::eResPlatform aPlatformTypes[zenResID::kePlatformType__Count];
	aPlatformTypes[zenResID::kePlatformType_OS]		= zenConst::kAWCurrentPlatformOS;
	aPlatformTypes[zenResID::kePlatformType_GFX]	= zenConst::kAWCurrentPlatformGfx;
	EMgr::Export.SetExportInfos( aPlatformTypes, zenConst::keResSource_Runtime, CreateExportItemRuntime );
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
zenRes::awResourceRef ManagerResource::GetResource(const zenResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());cd
	zenRes::awResource* pResource;
	if( mdResources.Get(_ResID.HashID(), pResource) )	return pResource;
	else if(_bSupportDefault )							return maResourcesDefault[_ResID.Type()];
	else												return (zenRes::awResource*)NULL;
}

//=================================================================================================
//! @brief		Find and return the resource with a particular zenResID
//! @details	This version try to look in both Loaded and Dynamic created resource.
//!				Useful for resource that can be dynamic and doesn't use an index for the name
//! @param		_ResID				- zenResID to look for
//! @param		_bSupportDefault	- If returning default resource of this type, when not found
//! @return 	Reference to resource
//=================================================================================================
zenRes::awResourceRef ManagerResource::GetResourceAnySource(const zenResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());
	zenRes::awResource* pResource;
	zenResID anySourceResID(_ResID);
	anySourceResID.SetSource(zenConst::keResSource_Loaded);
	if( mdResources.Get(_ResID.HashID(), pResource) )	
	{
		return pResource;
	}
	else
	{
		anySourceResID.SetSource(zenConst::keResSource_Runtime);	
		if( mdResources.Get(_ResID.HashID(), pResource) )	return pResource;
		else if(_bSupportDefault )							return maResourcesDefault[_ResID.Type()];
		else												return (zenRes::awResource*)NULL;
	}
}

//=================================================================================================
//! @brief		Add a resource to our resource dictionary
//! @details	This manager takes ownership of the resource and will free it once uneeded
//! @param		_pResource - Resource asset
//! @return 	Reference to resource
//=================================================================================================
zenRes::awResourceRef ManagerResource::Add(zenRes::awResource* _pResource)
{
	if( _pResource && _pResource->IsValid() )	
	{
		mdResources.Set(_pResource->GetResID().HashID(), _pResource);
		return _pResource;
	}
	return (zenRes::awResource*)NULL;
}
}