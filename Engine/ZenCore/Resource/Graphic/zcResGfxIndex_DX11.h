#pragma once
#ifndef __zCore_Res_Gfx_GfxIndex_DX11_h__
#define __zCore_Res_Gfx_GfxIndex_DX11_h__

namespace zcRes
{

//! @todo Urgent : Collapse Platform specific and data?
class GfxIndexData_DX11 : public zcExp::ExportGfxIndexDX11
{
public:
	ID3D11Buffer*					mpIndiceBuffer;
};

class GfxIndex_DX11 : public TResource2<class GfxIndex, GfxIndexData_DX11, zcExp::ExporterGfxIndexDX11_DX11>
{
ZENClassDeclare(GfxIndex_DX11, TResource2)
public:
	bool							Initialize();
	const zArrayStatic<zU8>&		GetIndices()const;
	zU8								GetIndiceSize()const;	//! @todo clean urgent 
	void							Update(zU8* _pData, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
};

typedef GfxIndex_DX11 GfxIndex_Platform;
typedef GfxIndexData_DX11 GfxIndex_Data; //SF test
}

#endif
