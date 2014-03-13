#pragma once
#ifndef __LibExport_Graphics_Texture_h__
#define __LibExport_Graphics_Texture_h__

namespace EExp
{
	struct TextureBlockInfo
	{
		zU8		muWidth;		// Width of a block
		zU8		muHeight;		// Height of a block
		zU16	muSize;			// Bytes size of a block
	};

	class SerialTexture2D_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialTexture2D_Base, EExp::ExportItem)
	public:
		struct MipInfo
		{
			MipInfo()
			: mvDim(0,0)
			, muStride(0)
			{}
			
			zArrayStatic<zU8>			maData;
			zVec2U16					mvDim;
			zU32						muStride;			
		};
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			zArrayStatic<MipInfo>		maMipData;
			zenConst::eTextureFormat	meFormatInput;	
			zenConst::eTextureFormat	meFormatOutput;
			zenFlagResTexCreate			mCreationFlags;
			zU8							mbGenerateMip;
		};
	};

	const TextureBlockInfo& GetTextureBlockInfo( zenConst::eTextureFormat _eFormat );	
	zResID CreateGfxTexture2D( zenConst::eTextureFormat _eFormat, zenFlagResTexCreate _CreationFlags, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData=zArrayStatic<zU8>(zUInt(0)) );

}

#include "EExpTextureDX11.h"

#endif