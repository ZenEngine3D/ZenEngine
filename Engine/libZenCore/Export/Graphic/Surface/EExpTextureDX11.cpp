#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	bool SerialGfxTexture2D_DX11::ExportWork(bool _bIsTHRTask)
	{
		ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);		
		mSerial.meFormat		= pExportInfo->meFormatOutput;
		mSerial.mCreationFlags	= pExportInfo->mCreationFlags;
		if( pExportInfo->mbGenerateMip )
		{
			//! @todo image conversion
			AWAssert( (mSerial.maMipData.Count()==1) && zenMath::IsPower2(mSerial.maMipData[0].mvDim.x) && zenMath::IsPower2(mSerial.maMipData[0].mvDim.y) );
		}
		
		if( pExportInfo->meFormatInput != pExportInfo->meFormatOutput )
		{
			//! @todo image conversion
			awUInt uMipCount = mSerial.maMipData.SetCount( pExportInfo->maMipData.Count() );
			for( awUInt mipIdx=0; mipIdx<uMipCount; ++mipIdx)
			{
				//..
			}					
		}
		else
		{
			awUInt uMipCount = mSerial.maMipData.SetCount( pExportInfo->maMipData.Count() );
			for(awUInt mipIdx(0); mipIdx<uMipCount; ++mipIdx)
			{
				mSerial.maMipData[mipIdx].maData	= pExportInfo->maMipData[mipIdx].maData;
				mSerial.maMipData[mipIdx].mvDim		= pExportInfo->maMipData[mipIdx].mvDim;
				mSerial.maMipData[mipIdx].muStride	= pExportInfo->maMipData[mipIdx].muStride;
			}
		}
		
		return TRUE;
	}
	
	bool SerialGfxTexture2D_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		if( Super::Serialize(_Serializer) )
			return false;

		return false;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
