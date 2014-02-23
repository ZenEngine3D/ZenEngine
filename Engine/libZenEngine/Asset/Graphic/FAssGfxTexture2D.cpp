#include "libZenEngine.h"
#if AW_ENGINETOOL

#define kuValue_Source "Source"

namespace FAss
{
	//=================================================================================================
	//! @brief		Get the list of properties definition for this asset
	//! @details	Used to know which data to read/write to xml
	//-------------------------------------------------------------------------------------------------
	//! @return		List of properties this asset needs
	//=================================================================================================
	const awArrayStatic<const FAss::PropertyDefBase*>& GfxTexture2D::GetPropertyDef() const
	{
		const FAss::PropertyDefEnum::Entry		enumEntries[]={	FAss::PropertyDefEnum::Entry(0, "Value0", "Enum Value 0"),	
																FAss::PropertyDefEnum::Entry(1, "Value1", "Enum Value 1"),	
																FAss::PropertyDefEnum::Entry(2, "Value2", "Enum Value 2"),	
																FAss::PropertyDefEnum::Entry(3, "Value3", "Enum Value 3"),	
																FAss::PropertyDefEnum::Entry(4, "Value4", "Enum Value 4"),	
																FAss::PropertyDefEnum::Entry(5, "Value5", "Enum Value 5")};
		
		static const FAss::PropertyDefBool		Property00("TestBool",		"", "Test Bool Field",		true,	false);		
		static const FAss::PropertyDefEnum		Property01("TestEnum",		"", "Test Enum Field",		true, awHash32("Value0"), enumEntries, AWArrayCount(enumEntries));
		static const FAss::PropertyDefFile		Property02(kuValue_Source,	"", "Texture file",			true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg");
		static const FAss::PropertyDefInt		Property03("TestIntA",		"", "Test Int Field",		true,	0, -10, 10,1);		
// 		static const AAss::PropertyDefInt2		Property03("TestInt2",		"", "Test Int2 Field",		true,	awVec2S32(0,1), -10, 10);
// 		static const AAss::PropertyDefInt3		Property04("TestInt3",		"", "Test Int3 Field",		true,	awVec3S32(0,1,2), -10, 10);
// 		static const AAss::PropertyDefInt4		Property05("TestInt4",		"", "Test Int4 Field",		true,	awVec4S32(0,1,2,3), -10, 10);
		static const FAss::PropertyDefFloat		Property06("TestFloat",		"", "Test Float Field",		true,	0.5, -10, 10);
 		static const FAss::PropertyDefFloat2	Property07("TestFloat2",	"", "Test Float2 Field",	true,	awVec2F(0,1.1f), awVec2F(-10,-5), awVec2F(10,5), awVec2F(1,0.5));
// 		static const AAss::PropertyDefFloat3	Property08("TestFloat3",	"", "Test Float3 Field",	true,	awVec3F(0,1.1f,2.2f), -10, 10);
// 		static const AAss::PropertyDefFloat4	Property09("TestFloat4",	"", "Test Float4 Field",	true,	awVec4F(0,1.1f,2.2f,3.3f), -10, 10);
		
		
		static const FAss::PropertyDefBase*		aPropertyAll[] = {	&Property00, &Property01, &Property02, &Property03, /*&Property04, 
																	&Property05,*/ &Property06, &Property07/*, &Property08, &Property09,
																	&Property10*/};
		static awArrayStatic<const FAss::PropertyDefBase*> saPropertyDef( aPropertyAll, AWArrayCount(aPropertyAll) );
		return saPropertyDef;		
	}

	//=================================================================================================
	//! @brief		Get index of PropertyDef with same name
	//! @details	Use fast lookup through a dictionary of all supported properties
	//-------------------------------------------------------------------------------------------------
	//! @param hPropertyName	- Name of property to look for
	//! @return					- Index of found item (0xFFFFFFFF if not found)
	//=================================================================================================
	awUInt GfxTexture2D::GetPropertyDefIndex(awHash32 _hPropertyName)const	
	{
		static Hashmap<awUInt>::Key32 sdPropertyIndex;
		if( !sdPropertyIndex.IsInit() )
			InitPropertyDefIndex(sdPropertyIndex);
		return sdPropertyIndex[_hPropertyName];
	}
}

#endif //AW_ENGINETOOL
