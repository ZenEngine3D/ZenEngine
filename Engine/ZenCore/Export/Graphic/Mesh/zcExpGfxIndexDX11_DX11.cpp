#include "zcCore.h"

namespace zcExp
{

ExporterGfxIndexDX11_DX11::ExporterGfxIndexDX11_DX11(const ResDataRef& _rResData)
: ExporterBase(_rResData.GetSafe())
, mrResData(_rResData)
{
}

bool ExporterGfxIndexDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ZENAssert(mrResData.IsValid());	
	ExportInfoGfxIndex* pExportInfo = static_cast<ExportInfoGfxIndex*>(mpExportInfo);
	
	if( pExportInfo->maIndice16.Count() > 0 )
	{
		mrResData->meIndiceFormat	= DXGI_FORMAT_R16_UINT;
		mrResData->muIndiceSize		= 2;
		mrResData->muIndiceCount		= pExportInfo->maIndice16.Count();;
		mrResData->maIndices.Copy( (zU8*)pExportInfo->maIndice16.First(), pExportInfo->maIndice16.SizeMem() );
	}
	else
	{
		mrResData->meIndiceFormat	= DXGI_FORMAT_R32_UINT;
		mrResData->muIndiceSize		= 4;		
		mrResData->muIndiceCount		= pExportInfo->maIndice32.Count();
		mrResData->maIndices.Copy( (zU8*)pExportInfo->maIndice32.First(), pExportInfo->maIndice32.SizeMem() );
	}
	
	switch( pExportInfo->mePrimitiveType )
	{
	case zenConst::kePrimType_PointList:		
		mrResData->mePrimitiveType	= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		mrResData->muPrimitiveCount	= mrResData->muIndiceCount;
		break;
	case zenConst::kePrimType_LineList:		
		mrResData->mePrimitiveType	= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		mrResData->muPrimitiveCount	= mrResData->muIndiceCount/2;
		break;
	case zenConst::kePrimType_LineStrip:		
		mrResData->mePrimitiveType	= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		mrResData->muPrimitiveCount	= mrResData->muIndiceCount-1;
		break;
	case zenConst::kePrimType_TriangleList:	
		mrResData->mePrimitiveType	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mrResData->muPrimitiveCount	= mrResData->muIndiceCount/3;
		break;
	case zenConst::kePrimType_TriangleStrip:	
		mrResData->mePrimitiveType	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		mrResData->muPrimitiveCount	= mrResData->muIndiceCount-2;
		break;
	default:
		ZENAssertMsg(0, "Unhandled Primitive type");				
		return FALSE;
	}
	
	return TRUE;
	
}
		
}

