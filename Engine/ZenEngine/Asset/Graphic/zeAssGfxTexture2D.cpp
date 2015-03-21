#include "zeEngine.h"
#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
	//=================================================================================================
	//! @brief		Get the list of properties definition for this asset
	//! @details	Used to know which data to read/write to xml
	//-------------------------------------------------------------------------------------------------
	//! @return		List of properties this asset needs
	//=================================================================================================
	const zenAss::PropertyDefArray& GfxTexture2D::GetProperties()const
	{ 	
		static zenAss::PropertyDefRef arPropertiesAll[] = {
			zenAss::PropertyFile::Create("Source",		"", "Texture file",		true,	zenT(""), zenT("Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg")),
			zenAss::PropertyFile::Create("Size",		"", "Texture file",		true,	zenT(""), zenT("Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg")),
		};
		static zenAss::PropertyDefArray sarProperties( arPropertiesAll, ZENArrayCount(arPropertiesAll) );
		return sarProperties;	
	}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
