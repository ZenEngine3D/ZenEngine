#include "libZenEngine.h"
#if ZEN_ENGINETOOL

#define kuValue_Source "Source"

namespace zen { namespace zeAss
{
#if 0
	//=================================================================================================
	//! @brief		Get the list of properties definition for this asset
	//! @details	Used to know which data to read/write to xml
	//-------------------------------------------------------------------------------------------------
	//! @return		List of properties this asset needs
	//=================================================================================================
	const zArrayStatic<const PropertyDefBase*>& zGfxTexture2D::GetPropertyDef() const
	{
#if 0
		const PropertyDefEnum::Entry	enumEntries[]={	PropertyDefEnum::Entry(0, "Value0", "Enum Value 0"),	
														PropertyDefEnum::Entry(1, "Value1", "Enum Value 1"),	
														PropertyDefEnum::Entry(2, "Value2", "Enum Value 2"),	
														PropertyDefEnum::Entry(3, "Value3", "Enum Value 3"),	
														PropertyDefEnum::Entry(4, "Value4", "Enum Value 4"),	
														PropertyDefEnum::Entry(5, "Value5", "Enum Value 5")};
		
		static const PropertyDefBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
		static const PropertyDefEnum	Property01("TestEnum",		"", "Test Enum Field",		true,	zHash32("Value0"), enumEntries, ZENArrayCount(enumEntries));
		static const PropertyDefFile	Property02(kuValue_Source,	"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
		static const PropertyDefInt		Property03("TestIntA",		"", "Test Int Field",		true,	0, -10, 10,1);		
// 		static const PropertyDefInt2	Property03("TestInt2",		"", "Test Int2 Field",		true,	zVec2S32(0,1), -10, 10);
// 		static const PropertyDefInt3	Property04("TestInt3",		"", "Test Int3 Field",		true,	zVec3S32(0,1,2), -10, 10);
// 		static const PropertyDefInt4	Property05("TestInt4",		"", "Test Int4 Field",		true,	zVec4S32(0,1,2,3), -10, 10);
		static const PropertyDefFloat	Property06("TestFloat",		"", "Test Float Field",		true,	0.5, -10, 10);
 		static const PropertyDefFloat2	Property07("TestFloat2",	"", "Test Float2 Field",	true,	zVec2F(0,1.1f), zVec2F(-10,-5), zVec2F(10,5), zVec2F(1,0.5));
// 		static const PropertyDefFloat3	Property08("TestFloat3",	"", "Test Float3 Field",	true,	zVec3F(0,1.1f,2.2f), -10, 10);
// 		static const PropertyDefFloat4	Property09("TestFloat4",	"", "Test Float4 Field",	true,	zVec4F(0,1.1f,2.2f,3.3f), -10, 10);
		
		
		static const PropertyDefBase*		aPropertyAll[] = {	&Property00, &Property01, &Property02, &Property03, /*&Property04, 
																	&Property05,*/ &Property06, &Property07/*, &Property08, &Property09,
																	&Property10*/};
#else
		static const PropertyDefFile	Property02(kuValue_Source,	"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
		static const PropertyDefBase*	aPropertyAll[] = {	&Property02 };
#endif
		
		static zArrayStatic<const PropertyDefBase*> saPropertyDef( aPropertyAll, ZENArrayCount(aPropertyAll) );
		return saPropertyDef;		
	}

	//=================================================================================================
	//! @brief		Get index of PropertyDef with same name
	//! @details	Use fast lookup through a dictionary of all supported properties
	//-------------------------------------------------------------------------------------------------
	//! @param hPropertyName	- Name of property to look for
	//! @return					- Index of found item (0xFFFFFFFF if not found)
	//=================================================================================================
	zUInt zGfxTexture2D::GetPropertyDefIndex(zHash32 _hPropertyName)const	
	{
		static zMap<zUInt>::Key32 sdPropertyIndex;
		if( !sdPropertyIndex.IsInit() )
			InitPropertyDefIndex(sdPropertyIndex);
		return sdPropertyIndex[_hPropertyName];
	}
#endif
	//=================================================================================================
	//! @brief		
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param hPropertyName	- Name of property to look for
	//! @return					- Index of found item (-1 if not found)
	//=================================================================================================
	zInt GfxTexture2D::GetValueIndex(zHash32 _hPropertyName)const	
	{
		static zMap<zInt>::Key32 sdPropertyIndex;
		static bool sbInit = InitPropertyMap(sdPropertyIndex);	//!< suEntryCount only used to init hashtable once
		return sdPropertyIndex[_hPropertyName];
	}

	const zenAss::zArrayProperty& GfxTexture2D::GetProperties()const
	{ 	
		//zMap<zU32>::Key32					mdPropertyIndex; todo
		static const zenAss::PropertyBool	Property00("TestBool",		"", "Test Bool Field",		true,	false);		
		static const zenAss::PropertyFile	Property02("Source",		"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
		static const zenAss::PropertyBase*	aPropertiesAll[] = {&Property00, &Property02 };
		static zenAss::zArrayProperty		saProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
		return saProperties;
	}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL
