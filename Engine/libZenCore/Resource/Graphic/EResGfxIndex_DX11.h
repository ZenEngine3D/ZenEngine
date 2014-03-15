#pragma once
#ifndef __LibResource_Graphics_GfxIndex_DX11_h__
#define __LibResource_Graphics_GfxIndex_DX11_h__

namespace ERes
{

class GfxIndex_DX11 : public Resource
{
struct InstanceInfo : public EExp::SerialGfxIndex_DX11
{
	ID3D11Buffer*				mpIndiceBuffer;
};
ZENResourceDeclare(GfxIndex_DX11, InstanceInfo, zenConst::keResType_GfxIndex)
public:
	virtual						~GfxIndex_DX11();
	virtual bool				ResourceInit();
//-----------------------------------------------------------------------------
// Common to all Vertex buffer
public:	
	zU8*						Lock();
	void						Unlock();
//-----------------------------------------------------------------------------
};

}

#endif