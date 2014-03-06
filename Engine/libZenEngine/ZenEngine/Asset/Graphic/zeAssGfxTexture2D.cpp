#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
	//=================================================================================================
	//! @brief		Get the list of properties definition for this asset
	//! @details	Used to know which data to read/write to xml
	//-------------------------------------------------------------------------------------------------
	//! @return		List of properties this asset needs
	//=================================================================================================
	const zenArrayStatic<const PropertyDefBase*>& GfxTexture2D::GetPropertyDef() const
	{	
		static const PropertyDefFile	PropSource("Source",	"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
		static const PropertyDefBase*	aPropertyAll[] = {	&PropSource };
		static zenArrayStatic<const PropertyDefBase*> saPropertyDef( aPropertyAll, ZENArrayCount(aPropertyAll) );
		return saPropertyDef;		
	}

	//=================================================================================================
	//! @brief		Get index of PropertyDef with same name
	//! @details	Use fast lookup through a dictionary of all supported properties
	//-------------------------------------------------------------------------------------------------
	//! @param hPropertyName	- Name of property to look for
	//! @return					- Index of found item (0xFFFFFFFF if not found)
	//=================================================================================================
	zenUInt GfxTexture2D::GetPropertyDefIndex(zenHash32 _hPropertyName)const	
	{
		static zenMap<zenUInt>::Key32 sdPropertyIndex;
		if( !sdPropertyIndex.IsInit() )
			InitPropertyDefIndex(sdPropertyIndex);
		return sdPropertyIndex[_hPropertyName];
	}
}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
