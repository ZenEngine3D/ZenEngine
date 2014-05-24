#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{

//=================================================================================================
//! @brief		Factory to create a new Asset of specified type
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param		_eAssetType	- Type of asset to create
//! @param		_Owner		- Owner package of the newly create Asset
//! @return					- Created Asset
//=================================================================================================
Asset* Asset::CreateItem( zenConst::eAssetType _eAssetType )
{
	Asset* pNewItem(NULL);
	switch( _eAssetType )
	{
	case zenConst::keAssType_TestProperty:	pNewItem = zenNewDefault TestProperty();	break;
	case zenConst::keAssType_Texture2D:		pNewItem = zenNewDefault GfxTexture2D();	break;
	case zenConst::keAssType_Mesh:			pNewItem = zenNewDefault GfxMesh();			break;
	default:								ZENAssertMsg(0, "Unsupported Asset Type");	break;
	}

	if( pNewItem )
	{
		pNewItem->InitDefault();
	}
	return pNewItem;
}

Asset::Asset()	
: mrPackage(NULL)
, muID(0)
{
	static zU32 sCounter(1);
	muID = sCounter++; //! @todo Asset: fix this (HACK)	
}

Asset::~Asset()	
{
}

void Asset::Init(zU32 _uID, const char* _zName, const char* _zGroup, Package& _ParentPkg)
{	
	zString::Split(_zGroup, '\\', maGroup, 1);
	*maGroup.Last() = _zName;

	muID		= _uID != 0 ? _uID :  zeMgr::Asset.GetAssetNextID( GetType() ); 
	mrPackage	= &_ParentPkg;

	SetPackage( &_ParentPkg );
	zeMgr::Asset.AssetAdd(this);
	//! @todo Asset : description init
	//RebuiltDescription();
}

void Asset::SetPackage(Package* _pParentPkg)
{
	if( mrPackage.IsValid() )
		mrPackage->AssetRem(*this);
	
	mrPackage = _pParentPkg;
	if( _pParentPkg )
		_pParentPkg->AssetAdd(*this);
}

//=================================================================================================
//! @brief		Build a string description based on asset property definitions/values
//! @details	Take all flagged properties and make a string description from it. Used for
//!				Asset short description in editor
//-------------------------------------------------------------------------------------------------
//! @return		
//=================================================================================================
void Asset::RebuiltDescription()
{	
	mzDescription = "";
	//! @TODO
	/*
	for(zUInt idx(0), count(maPropertyValueOld.Count()); idx<count; ++idx)
	{
		const PropertyDefBase& propDef = maPropertyValueOld[idx].GetBase()->mParentDef;
		if( propDef.mbShowInAssetDesc )
		{
			char zBuffer[128];			
			char zValue[64];
			maPropertyValueOld[idx].GetBase()->ValueToString( zValue, sizeof(zValue) );
			sprintf(zBuffer, "(%s : %s) ", propDef.mzDisplayName, zValue);
			mzDescription += zBuffer;
		}		
	}
	*/
}

void Asset::InitDefault()
{	
	const zenAss::PropertyDefArray& aProperties = GetProperties();	
	ZENAssertMsg(aProperties.Count() > 0, "An Asset type is missing ::GetProperties() implementation" );
	maPropertyValue.SetCount( aProperties.Count() );
	const zenAss::PropertyDefRef*	prDefinitionCur	= aProperties.First();
	zenAss::PropertyValueRef*		pValueCur		= maPropertyValue.First();
	zenAss::PropertyValueRef*		pValueLast		= maPropertyValue.Last();
	while( pValueCur <= pValueLast )
	{
		*pValueCur = (*prDefinitionCur)->Allocate();
		++pValueCur;
		++prDefinitionCur;
	}
}

const zenAss::PropertyDefArray& TestProperty::GetProperties()const
{ 	
 	const zenAss::PropertyEnum::Entry aEnumEntries[]={
 		zenAss::PropertyEnum::Entry(0, "Value A", "Description of 1st Entry"),
 		zenAss::PropertyEnum::Entry(1, "Value B", "Description of 2nd Entry"),
 		zenAss::PropertyEnum::Entry(2, "Value C", "Description of 3rd Entry"),
 		zenAss::PropertyEnum::Entry(3, "Value D", "Description of 4th Entry")};
	
 	static const zenAss::PropertyDefRef aPropertiesStruct[] =	{
		zenAss::PropertyBool::Create	("StructBool",	"", "Struct Test: Bool",	true,	false),
		zenAss::PropertyFloat::Create	("StructFloat",	"", "Struct Test: Float",	true,	1.f, -5.f, 5.f, 0.5f),
		zenAss::PropertyFloat2::Create	("StructFloat2","", "Struct Test: Float2",	true,	zVec2F(1.f), zVec2F(-10.f), zVec2F(10.f), zVec2F(0.1f))
	};

	static zenAss::PropertyDefRef rPropertyArrayStruct = zenAss::PropertyStruct::Create("PropertyStruct",	"",	"Test: Struct in Array", true, aPropertiesStruct, ZENArrayCount(aPropertiesStruct) );

	static const zenAss::PropertyDefRef aPropertiesAll[] = {
		zenAss::PropertyBool::Create	("Bool",	"", "Test: Bool",	true,	false),
		zenAss::PropertyFloat::Create	("Float",	"", "Test: Float",	true,	0.f, -10.f, 10.f, 0.5f),
		zenAss::PropertyFloat2::Create	("Float2",	"", "Test: Float2",	true,	zVec2F(1.f), zVec2F(-10.f), zVec2F(10.f), zVec2F(0.1f)),
		zenAss::PropertyFloat3::Create	("Float3",	"", "Test: Float3",	true,	zVec3F(1.f), zVec3F(-10.f), zVec3F(10.f), zVec3F(0.1f)),
		zenAss::PropertyFloat4::Create	("Float4",	"", "Test: Float4",	true,	zVec4F(1.f), zVec4F(-10.f), zVec4F(10.f), zVec4F(0.1f)),	
		zenAss::PropertyInt::Create		("Int",		"", "Test: Int",	true,	0, -10, 10, 1),
		zenAss::PropertyInt2::Create	("Int2",	"", "Test: Int2",	true,	zVec2S32(1), zVec2S32(-10), zVec2S32(10), zVec2S32(1)),
		zenAss::PropertyInt3::Create	("Int3",	"", "Test: Int3",	true,	zVec3S32(1), zVec3S32(-10), zVec3S32(10), zVec3S32(1)),
		zenAss::PropertyInt4::Create	("Int4",	"", "Test: Int4",	true,	zVec4S32(1), zVec4S32(-10), zVec4S32(10), zVec4S32(1)),
		zenAss::PropertyEnum::Create	("Enum",	"", "Test: Enum",	true,	0, aEnumEntries, ZENArrayCount(aEnumEntries) ),
		zenAss::PropertyFile::Create	("File",	"", "Test: File",	true,	"C:\\temp\\test.txt", "Images|*.bmp;*.png;*.jpeg;*.jpg|BMP(*.bmp)|*.bmp|PNG(*.png)|*.png|JPEG(*.jpg;*.jpeg)|*.jpg;*.jpeg"),
		zenAss::PropertyStruct::Create	("Struct",	"",	"Test: Struct", true,	aPropertiesStruct, ZENArrayCount(aPropertiesStruct) ),
		zenAss::PropertyArray::Create	("Array",	"", "Test: Array",	true,	rPropertyArrayStruct, 1),
	};
	static zenAss::PropertyDefArray		sarProperties( aPropertiesAll, ZENArrayCount(aPropertiesAll) );
	return sarProperties;
}

}} //namespace zen { namespace zeAss

#endif //ZEN_ENGINETOOL