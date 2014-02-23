#include "libZenCore.h"

namespace EExp
{
//=================================================================================================
//! @brief		Return basic information about smallest data unit of a texture format
//! @details	For some texture, it's 1 pixel, other format are 4x4 block, etc...
//-------------------------------------------------------------------------------------------------
//! @return 	- Texture Block info 
//=================================================================================================
const TextureBlockInfo& GetTextureBlockInfo( awconst::eTextureFormat _eFormat )
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
	AWStaticAssert(AWArrayCount(aTextureInfos)==awconst::keTexFormat__Count);
	AWAssert(_eFormat<awconst::keTexFormat__Count);
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
//! @return 				- Unique awResourceID of created Resource
//=================================================================================================
awResourceID CreateGfxTexture2D(awconst::eTextureFormat _eFormat, awFlagResTexCreate _CreationFlags, awVec2U16 _vDim, const awArrayBase<awU8>& _aRawData)
{
	static CMem::PoolAllocator sMemPool("Pool CreateTexture2D", sizeof(SerialTexture2D_Base::ExportInfo), 1, 5 );
	SerialTexture2D_Base::ExportInfo* pExportInfo	= AWNew(&sMemPool) SerialTexture2D_Base::ExportInfo;
	const EExp::TextureBlockInfo& BlockInfo			= EExp::GetTextureBlockInfo(_eFormat);
	pExportInfo->maMipData.SetCount(1);
	pExportInfo->maMipData[0].maData				= _aRawData;
	pExportInfo->maMipData[0].mvDim					= _vDim;
	pExportInfo->maMipData[0].muStride				= (_vDim.x / BlockInfo.muWidth) * BlockInfo.muSize;
	pExportInfo->meFormatInput						= _eFormat;	
	pExportInfo->meFormatOutput						= _eFormat;
	pExportInfo->mCreationFlags						= _CreationFlags;
	pExportInfo->mbGenerateMip						= FALSE;
	return EMgr::Export.CreateItem( awResourceID::kePlatformType_GFX, awconst::keResType_GfxTexture2D, pExportInfo );
}

}

