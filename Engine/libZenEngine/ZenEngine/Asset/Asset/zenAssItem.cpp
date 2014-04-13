#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

static const zStringHash32 sTypeDescription[]={
				zStringHash32("TestProperty"),	//keType_TestProperty,
				zStringHash32("Texture2D"),		//keType_Texture2D,
				zStringHash32("Mesh"),			//keType_Mesh,
};

zAssetItem AssetCreate(zenConst::eAssetType _eAssetType)
{
	zeAss::Asset* pNewItem(NULL);
	switch( _eAssetType )
	{
	case zenConst::keAssType_TestProperty:	pNewItem = zenNewDefault zeAss::TestProperty();	break;
	case zenConst::keAssType_Texture2D:		pNewItem = zenNewDefault zeAss::GfxTexture2D();	break;
	case zenConst::keAssType_Mesh:			pNewItem = zenNewDefault zeAss::GfxMesh();		break;
	default:								ZENAssertMsg(0, "Unsupported Asset Type");	break;
	}

	if( pNewItem )
		pNewItem->InitDefault();

	return pNewItem;
}

const char* AssetTypeToString(zenConst::eAssetType _eAssetType)
{
	ZENAssert(_eAssetType<zenConst::keAssType__Count);
	return sTypeDescription[_eAssetType].mzName;
}

zenConst::eAssetType AssetNameToType(zHash32 _hAssetName)
{
	zUInt idx = zStringHash32::Find(_hAssetName, sTypeDescription, ZENArrayCount(sTypeDescription));
	return idx < zenConst::keAssType__Count ? static_cast<zenConst::eAssetType>(idx) : zenConst::keAssType__Invalid;
}

const zAssetItem& AssetGet( zenConst::eAssetType _eAssetType, zU32 _uAssetID )
{
	return zeMgr::Asset.AssetGet(_eAssetType, _uAssetID);
}

zAssetItem::zAssetItem()
:Super()
{
}

zAssetItem::zAssetItem(const zAssetItem& _Copy)
: Super(_Copy.mpReference)
{
}

zAssetItem::zAssetItem(zeAss::Asset* _pAsset)
: Super(_pAsset)
{
}

zAssetItem& zAssetItem::operator=(const zAssetItem& _Copy)
{
	Super::operator=(_Copy.mpReference);
	return *this;
}

zU32 zAssetItem::GetID()const								
{ 
	ZENAssert(mpReference);	
	return mpReference->GetID(); 
}
zenConst::eAssetType zAssetItem::GetType()const
{
	ZENAssert(mpReference);	
	return mpReference->GetType(); 
}
const zString& zAssetItem::GetName()const						
{ 
	ZENAssert(mpReference);	
	return mpReference->GetName(); 
}
const zArrayStatic<zString>& zAssetItem::GetGroupAndName()const 
{ 
	ZENAssert(mpReference);	
	return mpReference->GetGroupAndName(); 
}
/*
zHash32 zAssetItem::GetGroupID()const							
{ 
	ZENAssert(mpReference);	
	return mpReference->GetGroupID(); 
}*/

const zString& zAssetItem::GetDescription()const				
{ 
	ZENAssert(mpReference);	
	return mpReference->GetDescription(); 
}
zUInt zAssetItem::GetValueCount()const							
{ 
	ZENAssert(mpReference);	
	return mpReference->GetValueCount(); 
}
zenAss::PropertyValue& zAssetItem::GetValue(zUInt _uValIndex)	
{ 
	ZENAssert(mpReference);	
	return mpReference->GetValue(_uValIndex); 
}
void zAssetItem::InitDefault()									
{ 
	ZENAssert(mpReference);	
	return mpReference->InitDefault(); 
}

void zAssetItem::Delete()
{
	ZENAssert(mpReference);
	zeMgr::Asset.AssetRem( mpReference->GetType(), mpReference->GetID() );
	*this = NULL;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
