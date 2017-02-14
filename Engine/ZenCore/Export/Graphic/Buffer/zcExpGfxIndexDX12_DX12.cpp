#include "zcCore.h"

namespace zcExp
{

ExporterGfxIndexDX12_DX12::ExporterGfxIndexDX12_DX12(const ExportDataRef& _rExportData)
: ExporterBase(_rExportData.GetSafe())
, mrExportData(_rExportData)
{
	zenAssert(mrExportData.IsValid());
}

bool ExporterGfxIndexDX12_DX12::ExportWork(bool _bIsTHRTask)
{	
	ExportInfoGfxIndex* pExportInfo = static_cast<ExportInfoGfxIndex*>(mpExportInfo);
	if( pExportInfo->maIndice16.Count() > 0 )
	{
		mrExportData->meIndiceFormat	= DXGI_FORMAT_R16_UINT;
		mrExportData->muIndiceSize		= 2;
		mrExportData->muIndiceCount		= static_cast<zU32>(pExportInfo->maIndice16.Count());
		mrExportData->maIndices.Copy( (zU8*)pExportInfo->maIndice16.First(), pExportInfo->maIndice16.SizeMem() );
	}
	else
	{
		mrExportData->meIndiceFormat	= DXGI_FORMAT_R32_UINT;
		mrExportData->muIndiceSize		= 4;		
		mrExportData->muIndiceCount		= static_cast<zU32>(pExportInfo->maIndice32.Count());
		mrExportData->maIndices.Copy( (zU8*)pExportInfo->maIndice32.First(), pExportInfo->maIndice32.SizeMem() );
	}
	
	switch( pExportInfo->mePrimitiveType )
	{
	case zenConst::kePrimType_PointList:			
		mrExportData->mePrimitive		= D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		mrExportData->mePrimitiveType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		mrExportData->muPrimitiveCount	= mrExportData->muIndiceCount;
		break;
	case zenConst::kePrimType_LineList:		
		mrExportData->mePrimitive		= D3D_PRIMITIVE_TOPOLOGY_LINELIST;
		mrExportData->mePrimitiveType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		mrExportData->muPrimitiveCount	= mrExportData->muIndiceCount/2;
		break;
	case zenConst::kePrimType_LineStrip:		
		mrExportData->mePrimitive		= D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
		mrExportData->mePrimitiveType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		mrExportData->muPrimitiveCount	= mrExportData->muIndiceCount-1;
		break;
	case zenConst::kePrimType_TriangleList:	
		mrExportData->mePrimitive		= D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		mrExportData->mePrimitiveType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		mrExportData->muPrimitiveCount	= mrExportData->muIndiceCount/3;
		break;
	case zenConst::kePrimType_TriangleStrip:	
		mrExportData->mePrimitive		= D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		mrExportData->mePrimitiveType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		mrExportData->muPrimitiveCount	= mrExportData->muIndiceCount-2;
		break;
	default:
		zenAssertMsg(0, "Unhandled Primitive type");				
		return FALSE;
	}
	return TRUE;	
}
		
}

