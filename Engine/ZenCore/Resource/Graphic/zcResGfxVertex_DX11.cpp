#include "zcCore.h"

namespace zcRes
{

GfxVertexHAL_DX11::~GfxVertexHAL_DX11()
{
	for(zUInt streamIdx=0; streamIdx<maStreamBuffer.Count(); ++streamIdx)
		maStreamBuffer[streamIdx]->Release();
	
	maStreamBuffer.Clear();
}

bool GfxVertexHAL_DX11::Initialize()
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
	zenStaticAssert( zenArrayCount(zSemanticNames)==zenConst::keShaderSemantic__Count );
	
	//! @todo Missing: configure resource creations flags
	D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
	UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);

	// Vertex Streams buffer init
	zUInt uStreamCount = maStreamBuffer.SetCount( maStream.Count() );
	maStreamStride.SetCount( maStream.Count() );
	for(zUInt streamIdx=0; streamIdx<uStreamCount; ++streamIdx)
	{
		const zcExp::ExportGfxVertex::Stream& stream	= maStream[streamIdx];			
		maStreamStride[streamIdx]						= stream.muStride;
						
		// Vertex Buffer Data
		D3D11_BUFFER_DESC vertexDesc;
		vertexDesc.ByteWidth							= static_cast<UINT>(stream.maData.SizeMem());
		vertexDesc.Usage								= eUsage;
		vertexDesc.BindFlags							= D3D11_BIND_VERTEX_BUFFER;		
		vertexDesc.CPUAccessFlags						= uCpuAccess;					
		vertexDesc.MiscFlags							= 0;	
		vertexDesc.StructureByteStride					= 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem								= stream.maData.First();
		initData.SysMemPitch							= 0;
		initData.SysMemSlicePitch						= 0;
		HRESULT hr										= zcMgr::GfxRender.DX11GetDevice()->CreateBuffer( &vertexDesc, &initData, &maStreamBuffer[streamIdx] );
		if( FAILED(hr) )			
		{
			//zbLog::Log(zbLog::keLog_ResGeometry, "Failed creating DX11 VertexBuffer");
			return false;
		}
	}	
		
	// Vertex element Definitions
	for(zUInt elemIdx=0, elemCount(maElementDef.Count()); elemIdx<elemCount; ++elemIdx)
		maElementDef[elemIdx].SemanticName = zSemanticNames[(zUInt)maElementDef[elemIdx].SemanticName];	//Convert Semantic Name index to string		
		
	return true;
}

void GfxVertexHAL_DX11::Update(zU8* _pData, zUInt _uOffset, zUInt _uSize)
{
	zenAssert(maStreamBuffer[0] != nullptr);
	D3D11_MAPPED_SUBRESOURCE mapRes;
	_uOffset		= 0;//zenMath::Min(_uOffset, (zUInt)muIndiceCount); //! @todo Urgent support partial updates
	_uSize			= zenMath::Min(_uSize, (zUInt)maStream[0].maData.SizeMem() - _uOffset);
	HRESULT result	= zcMgr::GfxRender.DX11GetDeviceContext()->Map(maStreamBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	memcpy((zU8*)mapRes.pData, _pData, _uSize); 
	zcMgr::GfxRender.DX11GetDeviceContext()->Unmap(maStreamBuffer[0], 0);
}

}
