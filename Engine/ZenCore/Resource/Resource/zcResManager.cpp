#include "zcCore.h"

namespace EMgr { zcRes::ManagerResource Resources; }

namespace zcRes
{
//=================================================================================================
//! @brief		Create new dynamicly create resources , at runtime.
//! @details	
//! @param		_eResType
//! @param		_pExportInfo
//! @return 	
//=================================================================================================
zResID RuntimeCreateResource(zcExp::ExportInfoBase* _pExportInfo)
{
	ZENAssert(_pExportInfo);
	zenRes::zResourceRef rResource;
	switch( _pExportInfo->mExportResID.Type() )
	{	
//	case zenConst::keResType_GfxShaderParamDef:			rResource = zGfxShaderParamDef::Create(*_pExportInfo);		break;
//	case zenConst::keResType_GfxShaderParam:			rResource = zGfxShaderParam::Create(*_pExportInfo);			break;
//	case zenConst::keResType_GfxShaderBinding:			rResource = zGfxShaderBinding::Create(*_pExportInfo);		break;
//	case zenConst::keResType_GfxIndex:					rResource = zGfxIndex::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxVertex:					rResource = zGfxVertex::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxTexture2D:				rResource = zGfxTexture2D::Create(*_pExportInfo);			break;
//	case zenConst::keResType_GfxRenderTarget:			rResource = zGfxRenderTarget::Create(*_pExportInfo);		break;
//	case zenConst::keResType_GfxView:					rResource = zGfxView::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxWindow:					rResource = zGfxWindow::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxMeshStrip:				rResource = zGfxMeshStrip::Create(*_pExportInfo);			break;
//	case zenConst::keResType_GfxMesh:					rResource = zGfxMesh::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxInputStream:			rResource = zGfxInputStream::Create(*_pExportInfo);			break;
//	case zenConst::keResType_GfxInputSignature:			rResource = zGfxInputSignature::Create(*_pExportInfo);		break;	
//	case zenConst::keResType_GfxBlend:					rResource = zGfxBlend::Create(*_pExportInfo);				break;
//	case zenConst::keResType_GfxDepthStencil:			rResource = zGfxDepthStencil::Create(*_pExportInfo);		break;
//	case zenConst::keResType_GfxRasterizer:				rResource = zGfxRasterizer::Create(*_pExportInfo);			break;

	case zenConst::keResType_GfxSampler:				rResource = GfxSampler::RuntimeExport(*_pExportInfo);			break;
	case zenConst::keResType_GfxBlend:					rResource = GfxStateBlend::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxDepthStencil:			rResource = GfxStateDepthStencil::RuntimeExport(*_pExportInfo);	break;
	case zenConst::keResType_GfxRasterizer:				rResource = GfxStateRasterizer::RuntimeExport(*_pExportInfo);	break;
	case zenConst::keResType_GfxShaderPixel:			rResource = GfxShaderPixel::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderVertex:			rResource = GfxShaderVertex::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxIndex:					rResource = GfxIndex::RuntimeExport(*_pExportInfo);				break;
	case zenConst::keResType_GfxVertex:					rResource = GfxVertex::RuntimeExport(*_pExportInfo);			break;
	case zenConst::keResType_GfxView:					rResource = GfxView::RuntimeExport(*_pExportInfo);				break;
	case zenConst::keResType_GfxRenderTarget:			rResource = GfxRenderTarget::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxTexture2D:				rResource = GfxTexture2d::RuntimeExport(*_pExportInfo);			break;
	case zenConst::keResType_GfxInputStream:			rResource = GfxInputStream::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxInputSignature:			rResource = GfxInputSignature::RuntimeExport(*_pExportInfo);	break;
	case zenConst::keResType_GfxMeshStrip:				rResource = GfxMeshStrip::RuntimeExport(*_pExportInfo);			break;
	case zenConst::keResType_GfxMesh:					rResource = GfxMesh::RuntimeExport(*_pExportInfo);				break;
	case zenConst::keResType_GfxShaderParamDef:			rResource = GfxShaderParamDef::RuntimeExport(*_pExportInfo);	break;
	case zenConst::keResType_GfxShaderParam:			rResource = GfxShaderParam::RuntimeExport(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderBinding:			rResource = GfxShaderBinding::RuntimeExport(*_pExportInfo);		break;
	}
	zenDelNull(_pExportInfo);
	return rResource.GetResID();
}

//=================================================================================================
//! @brief		Called at engine start
//! @return 	True if no error occurred
//! @todo Optim: Properly evaluate pre-allocation / growth policy
//=================================================================================================
bool ManagerResource::Load()
{		
	mdResources.Init(1024);	
	zenMem::Set(maResourcesDefault, sizeof(maResourcesDefault), 0);	
	zenConst::eResPlatform aPlatformTypes[zResID::kePlatformType__Count];
	aPlatformTypes[zResID::kePlatformType_OS]	= zenConst::kCurrentPlatformOS;
	aPlatformTypes[zResID::kePlatformType_GFX]	= zenConst::kCurrentPlatformGfx;
	EMgr::Export.SetExportInfos( aPlatformTypes, zenConst::keResSource_Runtime, RuntimeCreateResource );
	return true;
}

//=================================================================================================
//! @brief		Called at engine end to free up resources
//! @return 	True if no error occurred
//! @todo Missing: Unload resources
//=================================================================================================
bool ManagerResource::Unload()
{
	return true;
}

//=================================================================================================
//! @brief		Find and return the resource with a particular zResID
//! @details	
//! @param		_ResID				- zResID to look for
//! @param		_bSupportDefault	- If returning default resource of this type, when not found
//! @return 	Reference to resource
//=================================================================================================
zenRes::zResourceRef ManagerResource::GetResource(const zResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());cd
	zenRes::zResource* pResource;
	if( mdResources.Get(_ResID.HashID(), pResource) )	return pResource;
	else if(_bSupportDefault )							return maResourcesDefault[_ResID.Type()];
	else												return (zenRes::zResource*)NULL;
}

//=================================================================================================
//! @brief		Find and return the resource with a particular zResID
//! @details	This version try to look in both Loaded and Dynamic created resource.
//!				Useful for resource that can be dynamic and doesn't use an index for the name
//! @param		_ResID				- zResID to look for
//! @param		_bSupportDefault	- If returning default resource of this type, when not found
//! @return 	Reference to resource
//=================================================================================================
zenRes::zResourceRef ManagerResource::GetResourceAnySource(const zResID& _ResID, bool _bSupportDefault)
{
	//ZENAssert(_ResID.IsValid());
	zenRes::zResource* pResource;
	zResID anySourceResID(_ResID);
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
		else												return (zenRes::zResource*)NULL;
	}
}

//=================================================================================================
//! @brief		Add a resource to our resource dictionary
//! @details	This manager takes ownership of the resource and will free it once uneeded
//! @param		_pResource - Resource asset
//! @return 	Reference to resource
//=================================================================================================
zenRes::zResourceRef ManagerResource::Add(zenRes::zResource* _pResource)
{
	if( _pResource && _pResource->IsValid() )	
	{
		mdResources.Set(_pResource->GetResID().HashID(), _pResource);
		return _pResource;
	}
	return (zenRes::zResource*)NULL;
}
}