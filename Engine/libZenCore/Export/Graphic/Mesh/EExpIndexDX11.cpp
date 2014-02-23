#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
bool SerialGfxIndex_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);	
	if( pExportInfo->maIndice16.Count() > 0 )
	{
		meIndiceFormat	= DXGI_FORMAT_R16_UINT;
		muIndiceSize	= 2;
		muIndiceCount	= pExportInfo->maIndice16.Count();;
		mSerial.maIndices.Copy( (awU8*)pExportInfo->maIndice16.First(), pExportInfo->maIndice16.Size() );
	}
	else
	{
		meIndiceFormat	= DXGI_FORMAT_R32_UINT;
		muIndiceSize	= 4;		
		muIndiceCount	= pExportInfo->maIndice32.Count();
		mSerial.maIndices.Copy( (awU8*)pExportInfo->maIndice32.First(), pExportInfo->maIndice32.Size() );
	}
	
	switch( pExportInfo->mePrimitiveType )
	{
	case awconst::kePrimType_PointList:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		muPrimitiveCount	= muIndiceCount;
		break;
	case awconst::kePrimType_LineList:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		muPrimitiveCount	= muIndiceCount/2;
		break;
	case awconst::kePrimType_LineStrip:		
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		muPrimitiveCount	= muIndiceCount-1;
		break;
	case awconst::kePrimType_TriangleList:	
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		muPrimitiveCount	= muIndiceCount/3;
		break;
	case awconst::kePrimType_TriangleStrip:	
		mePrimitiveType		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		muPrimitiveCount	= muIndiceCount-2;
		break;
	default:
		AWAssertMsg(0, "Unhandled Primitive type");				
		return FALSE;
	}
	
	return TRUE;
}

bool SerialGfxIndex_DX11::Serialize( EExp::Serializer_Base& _Serializer )
{
	if( !SerialIndex_Base::Serialize(_Serializer) )
		return false;

	return true;
}


}
#endif //AW_EXPORT_OR_RESOURCE_DX11
