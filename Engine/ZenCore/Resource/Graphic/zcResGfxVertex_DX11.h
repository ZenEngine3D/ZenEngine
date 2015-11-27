#pragma once
#ifndef __zCore_Res_Gfx_GfxVertex_DX11_h__
#define __zCore_Res_Gfx_GfxVertex_DX11_h__

namespace zcRes
{

class GfxVertexProxy_DX11 : public zRefCounted
{
ZENClassDeclare(GfxVertexProxy_DX11, zRefCounted)
public:
											GfxVertexProxy_DX11();
	virtual									~GfxVertexProxy_DX11();												
	bool									Initialize(class GfxVertex& _Owner);
	zU8*									Lock();
	void									Unlock();

	struct StreamInfo
	{
		zU8	muElementStartIndex;							//!< Index of first stream element, in maElementDef
		zU8	muElementCount;									//!< stream elements count, in maElementDef
	};

	zArrayStatic<ID3D11Buffer*>				maStreamBuffer;	//!< D3D11 Buffer object
	zArrayStatic<UINT>						maStreamStride;	//!< Stride between 2 vertices of a buffer
	zArrayStatic<StreamInfo>				maStreamInfo;	//!< Element infos of stream
	zArrayStatic<D3D11_INPUT_ELEMENT_DESC>	maElementDef;	//!< D3D11 infos on each vertice element contained in all buffer (pos, uv, etc...)
	ZENDbgCode(class GfxVertex*		mpOwner);
};

}

#endif