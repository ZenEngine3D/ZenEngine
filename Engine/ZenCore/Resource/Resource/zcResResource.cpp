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
	case zenConst::keResType_GfxSampler:		rResource = RuntimeExport<GfxSamplerRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxShaderPixel:	rResource = RuntimeExport<GfxShaderPixelRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderVertex:	rResource = RuntimeExport<GfxShaderVertexRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxBlend:			rResource = RuntimeExport<GfxStateBlendRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxDepthStencil:	rResource = RuntimeExport<GfxStateDepthStencilRef>(*_pExportInfo);	break;
	case zenConst::keResType_GfxRaster:			rResource = RuntimeExport<GfxStateRasterRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxIndex:			rResource = RuntimeExport<GfxIndexRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxVertex:			rResource = RuntimeExport<GfxVertexRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxTarget2D:		rResource = RuntimeExport<GfxTarget2DRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxTexture2D:		rResource = RuntimeExport<GfxTexture2dRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxView:			rResource = RuntimeExport<GfxViewRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxInputSignature:	rResource = RuntimeExport<GfxInputSignatureRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderParamDef:	rResource = RuntimeExport<GfxShaderParamDefRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderParam:	rResource = RuntimeExport<GfxShaderParamRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxShaderBinding:	rResource = RuntimeExport<GfxShaderBindingRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxMeshStrip:		rResource = RuntimeExport<GfxMeshStripRef>(*_pExportInfo);			break;
	case zenConst::keResType_GfxMesh:			rResource = RuntimeExport<GfxMeshRef>(*_pExportInfo);				break;
	case zenConst::keResType_GfxInputStream:	rResource = RuntimeExport<GfxInputStreamRef>(*_pExportInfo);		break;
	case zenConst::keResType_GfxRenderPass:		rResource = RuntimeExport<GfxRenderPassRef>(*_pExportInfo);			break;
	}	
	return rResource.GetResID();
}

}