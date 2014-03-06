#pragma once
#ifndef __LibExport_Graphics_Texture_h__
#define __LibExport_Graphics_Texture_h__

namespace EExp
{
	struct TextureBlockInfo
	{
		zenU8	muWidth;		// Width of a block
		zenU8	muHeight;		// Height of a block
		zenU16	muSize;			// Bytes size of a block
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
			
			zenArrayStatic<zenU8>			maData;
			zenVec2U16					mvDim;
			zenU32						muStride;			
		};
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			zenArrayStatic<MipInfo>		maMipData;
			zenConst::eTextureFormat		meFormatInput;	
			zenConst::eTextureFormat		meFormatOutput;
			zenFlagResTexCreate			mCreationFlags;
			zenU8						mbGenerateMip;
		};
	};

	const TextureBlockInfo& GetTextureBlockInfo( zenConst::eTextureFormat _eFormat );	
	zenResID CreateGfxTexture2D( zenConst::eTextureFormat _eFormat, zenFlagResTexCreate _CreationFlags, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData=zenArrayStatic<zenU8>(zenUInt(0)) );

}

#include "EExpTextureDX11.h"

#endif