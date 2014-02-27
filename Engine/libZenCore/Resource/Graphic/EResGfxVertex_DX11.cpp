#include "libZenCore.h"

namespace ERes
{
	GfxVertex_DX11::GfxVertex_DX11()
	{
	}
	
	GfxVertex_DX11::~GfxVertex_DX11()
	{
		for(zenUInt streamIdx=0; streamIdx<mInstanceInfo.maStreamBuffer.Count(); ++streamIdx)
			mInstanceInfo.maStreamBuffer[streamIdx]->Release();
	}

	bool GfxVertex_DX11::ResourceInit()
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
		ZENStaticAssert( ZENArrayCount(zSemanticNames)==awconst::keShaderSemantic__Count );
	
		//D3D11_USAGE eUsage(D3D11_USAGE_DEFAULT);//! @todo configure this
		//uint uCpuAccess(0);
		D3D11_USAGE eUsage(D3D11_USAGE_DYNAMIC);
		UINT uCpuAccess(D3D11_CPU_ACCESS_WRITE);
		//! @todo
		//if( mInstanceInfo.eCreationFlags & SerialVertex_Base::keFlag_Static ) eUsage = D3D11_USAGE_IMMUTABLE;

		// Vertex Streams buffer init
		zenUInt uStreamCount = mInstanceInfo.maStreamBuffer.SetCount( mInstanceInfo.mSerial.maStream.Count() );
		mInstanceInfo.maStreamInfo.SetCount( mInstanceInfo.mSerial.maStream.Count() );
		mInstanceInfo.maStreamStride.SetCount( mInstanceInfo.mSerial.maStream.Count() );
		for(zenUInt streamIdx=0; streamIdx<uStreamCount; ++streamIdx)
		{
			const EExp::SerialGfxVertex_DX11::Stream& stream			= mInstanceInfo.mSerial.maStream[streamIdx];			
			mInstanceInfo.maStreamStride[streamIdx]						= stream.muStride;
			mInstanceInfo.maStreamInfo[streamIdx].muElementStartIndex	= stream.muElementStart;
			mInstanceInfo.maStreamInfo[streamIdx].muElementCount		= stream.muElementCount;
						
			// Vertex Buffer Data
			D3D11_BUFFER_DESC vertexDesc;
			vertexDesc.ByteWidth			= stream.maData.Size();
			vertexDesc.Usage				= eUsage;
			vertexDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;		
			vertexDesc.CPUAccessFlags		= uCpuAccess;					
			vertexDesc.MiscFlags			= 0;	
			vertexDesc.StructureByteStride	= 0;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem			= stream.maData.First();
			initData.SysMemPitch		= 0;
			initData.SysMemSlicePitch	= 0;
			HRESULT hr = EMgr::GfxRender.DX11GetDevice()->CreateBuffer( &vertexDesc, &initData, &mInstanceInfo.maStreamBuffer[streamIdx] );
			if( FAILED(hr) )
			{
				//CLog::Log(CLog::keLog_ResGeometry, "Failed creating DX11 VertexBuffer");
				int i=0;
			}
		}	
		
		// Vertex element Definitions
		zenUInt uElemDefCount = mInstanceInfo.maElementDef.Copy( (D3D11_INPUT_ELEMENT_DESC*)mInstanceInfo.mSerial.maElementDef.First(), mInstanceInfo.mSerial.maElementDef.Size() / sizeof(D3D11_INPUT_ELEMENT_DESC) );
		for(zenUInt elemIdx=0; elemIdx<uElemDefCount; ++elemIdx)
			mInstanceInfo.maElementDef[elemIdx].SemanticName = zSemanticNames[(zenUInt)mInstanceInfo.maElementDef[elemIdx].SemanticName];	//Convert Semantic Name index to string		
		
		return true;
	}

	zenU8* GfxVertex_DX11::Lock()
	{
		//! @todo some lock test here
		//! @todo Stream index parameter
		//! @todo specify access type
		if( Get().maStreamBuffer.Count() > 0 )
		{
			D3D11_MAPPED_SUBRESOURCE mapRes;
			HRESULT hr = EMgr::GfxRender.DX11GetDeviceContext()->Map(Get().maStreamBuffer[0], 0, D3D11_MAP_WRITE_DISCARD, NULL, &mapRes);
			if( FAILED(hr) )
			{
				int i=0;
			}
			return (zenU8*)mapRes.pData;
		}
		return NULL;
	}

	void GfxVertex_DX11::Unlock()
	{
		if( Get().maStreamBuffer.Count() > 0 )
		{
			EMgr::GfxRender.DX11GetDeviceContext()->Unmap(Get().maStreamBuffer[0], NULL);
		}
	}

}
