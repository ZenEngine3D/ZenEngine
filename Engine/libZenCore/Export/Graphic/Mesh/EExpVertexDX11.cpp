#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
bool SerialGfxVertex_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);
	zenUInt uSemanticIndex[]	= {	0,0,0,0,0,0,0,0 };
	const DXGI_FORMAT eFormats[awconst::keShaderElemType__Count][4]= {
		{DXGI_FORMAT_R32_FLOAT,	DXGI_FORMAT_R32G32_FLOAT,	DXGI_FORMAT_R32G32B32_FLOAT,	DXGI_FORMAT_R32G32B32A32_FLOAT},
		{DXGI_FORMAT_R32_SINT,	DXGI_FORMAT_R32G32_SINT,	DXGI_FORMAT_R32G32B32_SINT,		DXGI_FORMAT_R32G32B32A32_SINT},
		{DXGI_FORMAT_R32_UINT,	DXGI_FORMAT_R32G32_UINT,	DXGI_FORMAT_R32G32B32_UINT,		DXGI_FORMAT_R32G32B32A32_UINT}};	
	
	ZENStaticAssert( ZENArrayCount(uSemanticIndex)==awconst::keShaderSemantic__Count );
	
	zenU8 uElementTotal(0);
	zenU8 uElementCur(0);
	for(zenUInt i=0; i<pExportInfo->maStreams.Count(); ++i)
		uElementTotal += pExportInfo->maStreams[i].maElements.Count();

	mSerial.muCreationFlags = pExportInfo->muCreationFlags;
	mSerial.maStream.SetCount(pExportInfo->maStreams.Count());
	mSerial.maElementDef.SetCount(sizeof(D3D11_INPUT_ELEMENT_DESC)*uElementTotal);	
	D3D11_INPUT_ELEMENT_DESC* pElemDX11	= (D3D11_INPUT_ELEMENT_DESC*)mSerial.maElementDef.First();
	for( zenUInt stream=0; stream<mSerial.maStream.Count(); ++stream )
	{
		const zenRes::GfxVertex::Stream& StreamIn		= pExportInfo->maStreams[stream];
		EExp::SerialGfxVertex_DX11::Stream& StreamOut	= mSerial.maStream[stream];
		StreamOut.maData								= StreamIn.maData;
		StreamOut.muStride								= StreamIn.muStride;
		StreamOut.muElementStart						= uElementCur;
		StreamOut.muElementCount						= StreamIn.maElements.Count();		
		uElementCur										+= StreamIn.maElements.Count();
		for(zenUInt elem=0; elem<StreamOut.muElementCount; ++elem)
		{
			const zenRes::GfxVertex::Element& ElemInfo	= StreamIn.maElements[elem];			
			ZENAssert(ElemInfo.muVectorSize > 0);
			pElemDX11->SemanticName						= (const char*)ElemInfo.meSemantic;	// Will be converted to string pointer in Resource
			pElemDX11->SemanticIndex					= uSemanticIndex[ElemInfo.meSemantic]++;
			pElemDX11->Format							= eFormats[ElemInfo.meType][ElemInfo.muVectorSize-1];
			pElemDX11->AlignedByteOffset				= ElemInfo.muOffset;
			pElemDX11->InputSlot						= stream;
			pElemDX11->InputSlotClass					= D3D11_INPUT_PER_VERTEX_DATA;	//! @todo Support per intance streams
			pElemDX11->InstanceDataStepRate				= 0;
			++pElemDX11;
		}
	}
	return TRUE;
}

bool SerialGfxVertex_DX11::Stream::Serialize( EExp::Serializer_Base& _Serializer )
{
	return FALSE;
};

bool SerialGfxVertex_DX11::Serialize( EExp::Serializer_Base& _Serializer )
{
	if( !Super::Serialize(_Serializer) )
		return false;

	return false;
}
	

}

#endif //AW_EXPORT_OR_RESOURCE_DX11
