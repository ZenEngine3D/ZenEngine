#include "zcCore.h"

namespace zcExp
{
//=================================================================================================
//! @brief		Return basic information about smallest data unit of a texture format
//! @details	For some texture, it's 1 pixel, other format are 4x4 block, etc...
//-------------------------------------------------------------------------------------------------
//! @return 	- Texture Block info 
//=================================================================================================
const TextureBlockInfo& GetTextureBlockInfo( zenConst::eTextureFormat _eFormat )
{
	static const TextureBlockInfo aTextureInfos[] =
	{
		{1,		1,		1},		//keTexFormat_R8,		
		{1,		1,		3},		//keTexFormat_RGB8,
		{1,		1,		4},		//keTexFormat_RGBA8,
		{1,		1,		4},		//keTexFormat_D24S8,
		{1,		1,		4},		//keTexFormat_D32,		
		{4,		4,		8},		//keTexFormat_BC1,
		{4,		4,		8},		//keTexFormat_BC2,
		{4,		4,		8},		//keTexFormat_BC3,
		{4,		4,		16},	//keTexFormat_BC5,
		{4,		4,		16},	//keTexFormat_BC7,
		{1,		1,		32},	//keTexFormat_RGBA32f,
	};
	ZENStaticAssert(ZENArrayCount(aTextureInfos)==zenConst::keTexFormat__Count);
	ZENAssert(_eFormat<zenConst::keTexFormat__Count);
	return aTextureInfos[_eFormat];
}

//=================================================================================================
//! @brief		Create a new Texture Resource
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param _eFormat			- Format of raw data
//! @param _uCreationFlags	- Mask of TextureFlags used for texture creation
//! @param _vDim			- Width/Height of input image
//! @param _aRawData		- Image data in r/rgb/rgba format
//! @return 				- Unique zResID of created Resource
//=================================================================================================
zResID CreateGfxTexture2D(zenConst::eTextureFormat _eFormat, zFlagResTexUse _CreationFlags, zVec2U16 _vDim, const zArrayBase<zU8>& _aRawData)
{
	static zenMem::zAllocatorPool sMemPool("Pool CreateTexture2D", sizeof(ExportInfoGfxTexture2d), 1, 5 );
	ExportInfoGfxTexture2d* pExportInfo		= zenNew(&sMemPool) ExportInfoGfxTexture2d;
	const zcExp::TextureBlockInfo& BlockInfo	= zcExp::GetTextureBlockInfo(_eFormat);
	pExportInfo->maMipData.SetCount(1);
	pExportInfo->maMipData[0].maData			= _aRawData;
	pExportInfo->maMipData[0].mvDim				= _vDim;
	pExportInfo->maMipData[0].muStride			= (_vDim.x / BlockInfo.muWidth) * BlockInfo.muSize;
	pExportInfo->meFormatInput					= _eFormat;	
	pExportInfo->meFormatOutput					= _eFormat;
	pExportInfo->mCreationFlags					= _CreationFlags;
	pExportInfo->mbGenerateMip					= FALSE;
	return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxTexture2D, pExportInfo );
}

}

