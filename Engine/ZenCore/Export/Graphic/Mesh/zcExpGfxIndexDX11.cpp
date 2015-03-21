#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX11

namespace zcExp
{
bool SerialGfxIndex_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);	
	if( pExportInfo->maIndice16.Count() > 0 )
	{
		meIndiceFormat	= DXGI_FORMAT_R16_UINT;
		muIndiceSize	= 2;
		muIndiceCount	= pExportInfo->maIndice16.Count();;
		mSerial.maIndices.Copy( (zU8*)pExportInfo->maIndice16.First(), pExportInfo->maIndice16.Size() );
	}
	else
	{
		meIndiceFormat	= DXGI_FORMAT_R32_UINT;
		muIndiceSize	= 4;		
		muIndiceCount	= pExportInfo->maIndice32.Count();
		mSerial.maIndices.Copy( (zU8*)pExportInfo->maIndice32.First(), pExportInfo->maIndice32.Size() );
	}
	
	switch( pExportInfo->mePrimitiveType )
	{
	case zenConst::kePrimType_PointList:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		muPrimitiveCount	= muIndiceCount;
		break;
	case zenConst::kePrimType_LineList:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		muPrimitiveCount	= muIndiceCount/2;
		break;
	case zenConst::kePrimType_LineStrip:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		muPrimitiveCount	= muIndiceCount-1;
		break;
	case zenConst::kePrimType_TriangleList:	
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		muPrimitiveCount	= muIndiceCount/3;
		break;
	case zenConst::kePrimType_TriangleStrip:	
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		muPrimitiveCount	= muIndiceCount-2;
		break;
	default:
		ZENAssertMsg(0, "Unhandled Primitive type");				
		return FALSE;
	}
	
	return TRUE;
}

bool SerialGfxIndex_DX11::Serialize( zcExp::Serializer_Base& _Serializer )
{
	if( !SerialIndex_Base::Serialize(_Serializer) )
		return false;

	return true;
}


}
#endif //ZEN_EXPORT_OR_RESOURCE_DX11
