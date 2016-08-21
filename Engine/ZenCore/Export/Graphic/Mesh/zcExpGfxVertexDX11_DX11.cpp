#include "zcCore.h"

namespace zcExp
{

ExporterGfxVertexDX11_DX11::ExporterGfxVertexDX11_DX11(const ExportResultRef& _rExportOut)
: ExporterBase(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
	zenAssert(mrExport.IsValid());
}

bool ExporterGfxVertexDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxVertex* pExportInfo = static_cast<ExportInfoGfxVertex*>(mpExportInfo);
	zUInt uSemanticIndex[]	= {	0,0,0,0,0,0,0,0 };
	const DXGI_FORMAT eFormats[][4]= {
		{DXGI_FORMAT_R32_FLOAT,	DXGI_FORMAT_R32G32_FLOAT,	DXGI_FORMAT_R32G32B32_FLOAT,	DXGI_FORMAT_R32G32B32A32_FLOAT},
		{DXGI_FORMAT_R32_SINT,	DXGI_FORMAT_R32G32_SINT,	DXGI_FORMAT_R32G32B32_SINT,		DXGI_FORMAT_R32G32B32A32_SINT},
		{DXGI_FORMAT_R32_UINT,	DXGI_FORMAT_R32G32_UINT,	DXGI_FORMAT_R32G32B32_UINT,		DXGI_FORMAT_R32G32B32A32_UINT},
		{DXGI_FORMAT_R8_UNORM,	DXGI_FORMAT_R8G8_UNORM,		DXGI_FORMAT_UNKNOWN,			DXGI_FORMAT_R8G8B8A8_UNORM},
		{DXGI_FORMAT_R8_SNORM,	DXGI_FORMAT_R8G8_SNORM,		DXGI_FORMAT_UNKNOWN,			DXGI_FORMAT_R8G8B8A8_SNORM },
		};	
	
	zenStaticAssert( ZENArrayCount(uSemanticIndex)==zenConst::keShaderSemantic__Count );
	zenStaticAssert( sizeof(eFormats)/sizeof(DXGI_FORMAT)/4 == zenConst::keShaderElemType__Count);

	//! @todo Missing: configure resource creations flags
//	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	//UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);

	zU8 uElemOutTotal(0), uElemOutCur(0);

	for(zUInt i=0; i<pExportInfo->maStreams.Count(); ++i)
		uElemOutTotal += (zU8)pExportInfo->maStreams[i].maElements.Count();
	
	mrExport->maStream.SetCount(pExportInfo->maStreams.Count());
	mrExport->maElementDef.SetCount(uElemOutTotal);	
	mrExport->mResourceUse				= pExportInfo->mResourceUse;
	D3D11_INPUT_ELEMENT_DESC* pElements	= mrExport->maElementDef.First();

	for( zUInt stream=0; stream<mrExport->maStream.Count(); ++stream )
	{
		const zenRes::zGfxVertex::Stream& StreamIn		= pExportInfo->maStreams[stream];
		ExportGfxVertex::Stream& StreamOut				= mrExport->maStream[stream];
		StreamOut.maData								= StreamIn.maData;
		StreamOut.muStride								= StreamIn.muStride;
		StreamOut.muElementStart						= uElemOutCur;
		StreamOut.muElementCount						= (zU8)StreamIn.maElements.Count();		
		
		for(zUInt elem=0; elem<StreamOut.muElementCount; ++elem)
		{
			const zenRes::zGfxVertex::Element& ElemInfo	= StreamIn.maElements[elem];			
			zenAssert(ElemInfo.muVectorSize > 0);
			pElements[uElemOutCur].SemanticName			= (const char*)ElemInfo.meSemantic;	// Will be converted to string pointer in Resource
			pElements[uElemOutCur].SemanticIndex		= (UINT)uSemanticIndex[ElemInfo.meSemantic]++;
			pElements[uElemOutCur].Format				= eFormats[ElemInfo.meType][ElemInfo.muVectorSize-1];
			pElements[uElemOutCur].AlignedByteOffset	= ElemInfo.muOffset;
			pElements[uElemOutCur].InputSlot			= (UINT)stream;
			pElements[uElemOutCur].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;	//! @todo Missing: Support per intance streams
			pElements[uElemOutCur].InstanceDataStepRate	= 0;
			++uElemOutCur;
		}
	}
	return TRUE;
}

}
