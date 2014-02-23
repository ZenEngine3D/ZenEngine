#pragma once
#ifndef __LibExport_Graphics_Texture_h__
#define __LibExport_Graphics_Texture_h__

namespace EExp
{
	struct TextureBlockInfo
	{
		awU8	muWidth;		// Width of a block
		awU8	muHeight;		// Height of a block
		awU16	muSize;			// Bytes size of a block
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
			
			awArrayStatic<awU8>			maData;
			awVec2U16					mvDim;
			awU32						muStride;			
		};
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			awArrayStatic<MipInfo>		maMipData;
			awconst::eTextureFormat		meFormatInput;	
			awconst::eTextureFormat		meFormatOutput;
			awFlagResTexCreate			mCreationFlags;
			awU8						mbGenerateMip;
		};
	};

	const TextureBlockInfo& GetTextureBlockInfo( awconst::eTextureFormat _eFormat );	
	awResourceID CreateGfxTexture2D( awconst::eTextureFormat _eFormat, awFlagResTexCreate _CreationFlags, awVec2U16 _vDim, const awArrayBase<awU8>& _aRawData=awArrayStatic<awU8>(awUInt(0)) );

}

#include "EExpTextureDX11.h"

#endif