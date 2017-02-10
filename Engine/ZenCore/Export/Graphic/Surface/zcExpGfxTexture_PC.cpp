#include "zcCore.h"

namespace zcExp
{

bool GenerateTexture( zArrayStatic<MipInfo>& _aOutMips, const ExportInfoGfxTexture2D& _ExportInfo )
{
	if( _ExportInfo.meFormatInput != _ExportInfo.meFormatOutput )
	{
		//! @todo Missing: Image conversion
		zUInt uMipCount = _aOutMips.SetCount( _ExportInfo.maMipData.Count() );
		for( zUInt mipIdx=0; mipIdx<uMipCount; ++mipIdx)
		{
			//..
		}					
	}
		
	zUInt uMipCount = _aOutMips.SetCount( _ExportInfo.maMipData.Count() );
	for(zUInt mipIdx(0); mipIdx<uMipCount; ++mipIdx)
	{
		_aOutMips[mipIdx].maData	= _ExportInfo.maMipData[mipIdx].maData;
		_aOutMips[mipIdx].mvDim		= _ExportInfo.maMipData[mipIdx].mvDim;
		_aOutMips[mipIdx].muStride	= _ExportInfo.maMipData[mipIdx].muStride;
	}
	
	if( _ExportInfo.mbGenerateMip )
	{
		//! @todo Missing: Mipmap creation
		zenAssert(	(_aOutMips.Count()==1) && 
					zenMath::IsPower2(_aOutMips[0].mvDim.x) && 
					zenMath::IsPower2(_aOutMips[0].mvDim.y) );
	}

	return true;
}

}

