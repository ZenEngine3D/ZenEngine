#include "zcCore.h"

namespace zcRes
{

GfxVertexProxy_DX11::GfxVertexProxy_DX11()
{
}

GfxVertexProxy_DX11::~GfxVertexProxy_DX11()
{
	for(zUInt streamIdx=0; streamIdx<maStreamBuffer.Count(); ++streamIdx)
		maStreamBuffer[streamIdx]->Release();
	
	maStreamBuffer.Clear();
}

bool GfxVertexProxy_DX11::Initialize(class GfxVertex& _Owner)
{
	const char* zSemanticNames[]={
		"POSITION",		//keVertexSemantic_Position,
		"TEXCOORD",		//keVertexSemantic_UV,
		"COLOR",		//keVertexSemantic_Color,
		"BINORMAL",		//keVertexSemantic_Binormal
		"NORMAL",		//keVertexSemantic_Normal,
		"TANGENT",		//keVertexSemantic_Tangent,
		"BLENDINDICES",	//keVertexSemantic_BlendIndex,
		"BLENDWEIGHT",	//keVertexSemantic_BlendWeight,
	};
	ZENStaticAssert( ZENArrayCount(zSemanticNames)==zenConst::keShaderSemantic__Count );
	
	const GfxVertex::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);

	//! @todo Missing: configure resource creations flags
	//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);
	//zUInt uCpuAccess(0);
	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);

	// Vertex Streams buffer init
	zUInt uStreamCount = maStreamBuffer.SetCount( rResData->maStream.Count() );
	maStreamInfo.SetCount( rResData->maStream.Count() );
	maStreamStride.SetCount( rResData->maStream.Count() );
	for(zUInt streamIdx=0; streamIdx<uStreamCount; ++streamIdx)
	{
		const GfxVertexResData::Stream& stream		= rResData->maStream[streamIdx];			
		maStreamStride[streamIdx]						= stream.muStride;
		maStreamInfo[streamIdx].muElementStartIndex		= stream.muElementStart;
		maStreamInfo[streamIdx].muElementCount			= stream.muElementCount;
						
		// Vertex Buffer Data
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth			= static_cast<UINT>(stream.maData.SizeMem());
		vertexDesc.Usage				= eUsage;
		vertexDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;		
		vertexDesc.CPUAccessFlags		= uCpuAccess;					
		vertexDesc.MiscFlags			= 0;	
		vertexDesc.StructureByteStride	= 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem			= stream.maData.First();
		initData.SysMemPitch		= 0;
		initData.SysMemSlicePitch	= 0;
		HRESULT hr = zcMgr::GfxRender.DX11GetDevice()->CreateBuffer( &vertexDesc, &initData, &maStreamBuffer[streamIdx] );
		if( FAILED(hr) )
		{
			//zbLog::Log(zbLog::keLog_ResGeometry, "Failed creating DX11 VertexBuffer");
			int i=0;
		}
	}	
		
	// Vertex element Definitions
	zUInt uElemDefCount = maElementDef.Copy( (D3D11_INPUT_ELEMENT_DESC*)rResData->maElementDef.First(), rResData->maElementDef.SizeMem() / sizeof(D3D11_INPUT_ELEMENT_DESC) );
	for(zUInt elemIdx=0; elemIdx<uElemDefCount; ++elemIdx)
		maElementDef[elemIdx].SemanticName = zSemanticNames[(zUInt)maElementDef[elemIdx].SemanticName];	//Convert Semantic Name index to string		
		
	return true;
}

zU8* GfxVertexProxy_DX11::Lock()
{
	//! @todo Missing: Stream index parameter
	//! @todo Missing: specify access type
	if( maStreamBuffer.Count() > 0 )
	{
		D3D11_MAPPED_SUBRESOURCE mapRes;
		HRESULT hr = zcMgr::GfxRender.DX11GetDeviceContext()->Map(maStreamBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
		if( SUCCEEDED(hr) )
			return (zU8*)mapRes.pData;
	}
	return NULL;
}

void GfxVertexProxy_DX11::Unlock()
{
	if( maStreamBuffer.Count() > 0 )
	{
		zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(maStreamBuffer[0], NULL);
	}
}

}
