#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

zAssetItem AssetCreate(zenConst::eAssetType _eAssetType)
{
	zeAss::AssetItem* pNewItem(NULL);
	switch( _eAssetType )
	{
	case zenConst::keAssType_TestProperty:	pNewItem = zenNewDefault zeAss::TestProperty();	break;
	case zenConst::keAssType_Texture2D:		pNewItem = zenNewDefault zeAss::GfxTexture2D();	break;
	case zenConst::keAssType_Mesh:			pNewItem = zenNewDefault zeAss::GfxMesh();		break;
	default:								ZENAssertMsg(0, "Unsupported Asset Type");	break;
	}

	if( pNewItem )
	{
		pNewItem->InitDefault();
		//pNewItem->mpPackage = &_Owner;
	}
	return pNewItem;
}

zAssetItem::zAssetItem()
:Super()
{
}

zAssetItem::zAssetItem(const zAssetItem& _Copy)
: Super(_Copy.mpReference)
{
}

zAssetItem::zAssetItem(zeAss::AssetItem* _pAsset)
: Super(_pAsset)
{
}

zAssetItem& zAssetItem::operator=(const zAssetItem& _Copy)
{
	Super::operator=(_Copy.mpReference);
	return *this;
}

zHash64 zAssetItem::GetID()const								
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
zHash32 zAssetItem::GetGroupID()const							
{ 
	ZENAssert(mpReference);	
	return mpReference->GetGroupID(); 
}
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

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
