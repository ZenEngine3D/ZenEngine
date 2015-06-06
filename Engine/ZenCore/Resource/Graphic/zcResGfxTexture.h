#pragma once
#ifndef __zCore_Res_Gfx_Texture_h__
#define __zCore_Res_Gfx_Texture_h__

namespace zcRes
{

class GfxTexture2d : public TResource<GfxTexture2d, GfxTexture2dResData, GfxTexture2dProxy, GfxTexture2dExporterRuntime>
{
ZENClassDeclare(GfxTexture2d, TResource)
public:	
	ZENInline const zVec2U16& GetDim()
	{ 
		//! @todo cleanup save dim, dump exportdata
		return (mrResData->maMipData.Count() > 0 ) ? mrResData->maMipData[0].mvDim : zVec2U16::Zero; 
	}
};

}

#endif
