#include "zcCore.h"

namespace zcExp
{

ExporterGfxVertexDX11_DX11::ExporterGfxVertexDX11_DX11(const ResDataRef& _rResData)
: ExporterBase(_rResData.GetSafe())
, mrResData(_rResData)
{
}

bool ExporterGfxVertexDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ZENAssert(mrResData.IsValid());	
	ExportInfoGfxVertex* pExportInfo = static_cast<ExportInfoGfxVertex*>(mpExportInfo);
	zUInt uSemanticIndex[]	= {	0,0,0,0,0,0,0,0 };
	const DXGI_FORMAT eFormats[zenConst::keShaderElemType__Count][4]= {
		{DXGI_FORMAT_R32_FLOAT,	DXGI_FORMAT_R32G32_FLOAT,	DXGI_FORMAT_R32G32B32_FLOAT,	DXGI_FORMAT_R32G32B32A32_FLOAT},
		{DXGI_FORMAT_R32_SINT,	DXGI_FORMAT_R32G32_SINT,	DXGI_FORMAT_R32G32B32_SINT,		DXGI_FORMAT_R32G32B32A32_SINT},
		{DXGI_FORMAT_R32_UINT,	DXGI_FORMAT_R32G32_UINT,	DXGI_FORMAT_R32G32B32_UINT,		DXGI_FORMAT_R32G32B32A32_UINT}};	
	
	ZENStaticAssert( ZENArrayCount(uSemanticIndex)==zenConst::keShaderSemantic__Count );
	
	zU8 uElementTotal(0);
	zU8 uElementCur(0);
	for(zUInt i=0; i<pExportInfo->maStreams.Count(); ++i)
		uElementTotal += pExportInfo->maStreams[i].maElements.Count();

	mrResData->mResourceUse			= pExportInfo->mResourceUse;
	mrResData->maStream.SetCount(pExportInfo->maStreams.Count());
	mrResData->maElementDef.SetCount(sizeof(D3D11_INPUT_ELEMENT_DESC)*uElementTotal);	
	D3D11_INPUT_ELEMENT_DESC* pElemDX11	= (D3D11_INPUT_ELEMENT_DESC*)mrResData->maElementDef.First();
	for( zUInt stream=0; stream<mrResData->maStream.Count(); ++stream )
	{
		const zenRes::zGfxVertex::Stream& StreamIn		= pExportInfo->maStreams[stream];
		ResData::Stream& StreamOut					= mrResData->maStream[stream];
		StreamOut.maData								= StreamIn.maData;
		StreamOut.muStride								= StreamIn.muStride;
		StreamOut.muElementStart						= uElementCur;
		StreamOut.muElementCount						= StreamIn.maElements.Count();		
		uElementCur										+= StreamIn.maElements.Count();
		for(zUInt elem=0; elem<StreamOut.muElementCount; ++elem)
		{
			const zenRes::zGfxVertex::Element& ElemInfo	= StreamIn.maElements[elem];			
			ZENAssert(ElemInfo.muVectorSize > 0);
			pElemDX11->SemanticName						= (const char*)ElemInfo.meSemantic;	// Will be converted to string pointer in Resource
			pElemDX11->SemanticIndex					= uSemanticIndex[ElemInfo.meSemantic]++;
			pElemDX11->Format							= eFormats[ElemInfo.meType][ElemInfo.muVectorSize-1];
			pElemDX11->AlignedByteOffset				= ElemInfo.muOffset;
			pElemDX11->InputSlot						= stream;
			pElemDX11->InputSlotClass					= D3D11_INPUT_PER_VERTEX_DATA;	//! @todo Missing: Support per intance streams
			pElemDX11->InstanceDataStepRate				= 0;
			++pElemDX11;
		}
	}
	return TRUE;
}

}
