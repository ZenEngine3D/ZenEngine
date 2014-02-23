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
	AWClassDeclare(SerialTexture2D_Base, EExp::ExportItem)
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
			awconst::eTextureFormat		meFormatInput;	
			awconst::eTextureFormat		meFormatOutput;
			awFlagResTexCreate			mCreationFlags;
			zenU8						mbGenerateMip;
		};
	};

	const TextureBlockInfo& GetTextureBlockInfo( awconst::eTextureFormat _eFormat );	
	zenResID CreateGfxTexture2D( awconst::eTextureFormat _eFormat, awFlagResTexCreate _CreationFlags, zenVec2U16 _vDim, const zenArrayBase<zenU8>& _aRawData=zenArrayStatic<zenU8>(zenUInt(0)) );

}

#include "EExpTextureDX11.h"

#endif