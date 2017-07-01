#include "zcCore.h"

namespace zcRes
{	
//=================================================================================================
//! @brief		Create new dynamically create resources , at runtime.
//! @details	
//! @param		_eResType
//! @param		_pExportInfo
//! @return 	
//=================================================================================================
zResID RuntimeCreateResource(zcExp::ExportInfoBase* _pExportInfo)
{
	zenAssert(_pExportInfo);
	ResourceAnyRef rResource;
	switch( _pExportInfo->mExportResID.GetType() )
	{	
	case zenConst::keResType_GfxStateSampler:		rResource = RuntimeExport<GfxStateSamplerRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxShaderPixel:		rResource = RuntimeExport<GfxShaderPixelRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderVertex:		rResource = RuntimeExport<GfxShaderVertexRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxStateBlend:			rResource = RuntimeExport<GfxStateBlendRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxStateDepthStencil:	rResource = RuntimeExport<GfxStateDepthStencilRef>(*_pExportInfo);	break;
	case zenConst::keResType_GfxStateRaster:		rResource = RuntimeExport<GfxStateRasterRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxIndex:				rResource = RuntimeExport<GfxIndexRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxTarget2D:			rResource = RuntimeExport<GfxTarget2DRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxTexture2D:			rResource = RuntimeExport<GfxTexture2DRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxView:				rResource = RuntimeExport<GfxViewRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxCBufferDefinition:	rResource = RuntimeExport<GfxCBufferDefinitionRef>(*_pExportInfo);	break;
	case zenConst::keResType_GfxCBuffer:			rResource = RuntimeExport<GfxCBufferRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxShaderBinding:		rResource = RuntimeExport<GfxShaderBindingRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxMeshStrip:			rResource = RuntimeExport<GfxMeshStripRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxMesh:				rResource = RuntimeExport<GfxMeshRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxBuffer:				rResource = RuntimeExport<GfxBufferRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxRenderPass:			rResource = RuntimeExport<GfxRenderPassRef>(*_pExportInfo);			break;
	}	
	return rResource.GetResID();
}

}