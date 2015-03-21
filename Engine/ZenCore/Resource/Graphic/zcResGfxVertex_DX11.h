#pragma once
#ifndef __zCore_Res_Gfx_GfxVertex_DX11_h__
#define __zCore_Res_Gfx_GfxVertex_DX11_h__

namespace zcRes
{

class GfxVertex_DX11 : public Resource
{
	struct StreamInfo
	{
		zU8		muElementStartIndex;							//!< Index of first stream element, in maElementDef
		zU8		muElementCount;									//!< stream elements count, in maElementDef
	};

	struct InstanceInfo : public zcExp::SerialGfxVertex_DX11
	{
		zArrayStatic<ID3D11Buffer*>				maStreamBuffer;	//!< D3D11 Buffer object
		zArrayStatic<UINT>						maStreamStride;	//!< Stride between 2 vertices of a buffer
		zArrayStatic<StreamInfo>				maStreamInfo;	//!< Element infos of stream
		zArrayStatic<D3D11_INPUT_ELEMENT_DESC>	maElementDef;	//!< D3D11 infos on each vertice element contained in all buffer (pos, uv, etc...)
	};
ZENResourceDeclare(GfxVertex_DX11, InstanceInfo, zenConst::keResType_GfxVertex)
public:
	
	virtual										~GfxVertex_DX11();
	virtual bool								ResourceInit();

//-----------------------------------------------------------------------------
// Common to all Vertex buffer
public:	
	zU8*										Lock();
	void										Unlock();
//-----------------------------------------------------------------------------
};


}

#endif