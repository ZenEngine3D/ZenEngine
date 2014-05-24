#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

const zStringHash32 sTypeDescription[]={
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

const zAssetItem& AssetGet( zAssetItem::ID _uAssetID )
{
	return zeMgr::Asset.AssetGet(_uAssetID.meType, _uAssetID.muIndex);
}

zAssetItem::zAssetItem()
:Super()
{
}

zAssetItem::zAssetItem(const zAssetItem& _Copy)
: Super(_Copy)
{
}

zAssetItem::zAssetItem(zeAss::Asset* _pAsset)
: Super(_pAsset)
{
}

zAssetItem& zAssetItem::operator=(const zAssetItem& _Copy)
{
	Super::operator=(_Copy);
	return *this;
}

zAssetItem::ID zAssetItem::GetID()const								
{ 
	ZENAssert(IsValid());
	return ID(zEngineRefConst::Get()->GetType(), zEngineRefConst::Get()->GetID());
}
zenConst::eAssetType zAssetItem::GetType()const
{
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetType(); 
}
const zString& zAssetItem::GetName()const						
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetName(); 
}
const zArrayStatic<zString>& zAssetItem::GetGroupAndName()const 
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetGroupAndName(); 
}
/*
zHash32 zAssetItem::GetGroupID()const							
{ 
	ZENAssert(IsValid());
	return mpReference->GetGroupID(); 
}*/

const zString& zAssetItem::GetDescription()const				
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetDescription(); 
}
zUInt zAssetItem::GetValueCount()const							
{ 
	ZENAssert(IsValid());
	return zEngineRefConst::Get()->GetValueCount(); 
}
zenAss::PropertyValueRef zAssetItem::GetValue(zUInt _uValIndex)	
{ 
	ZENAssert(IsValid());
	return Get()->GetValue(_uValIndex); 
}
void zAssetItem::InitDefault()									
{ 
	ZENAssert(IsValid());
	return Get()->InitDefault(); 
}

void zAssetItem::Delete()
{
	ZENAssert(IsValid());
	zeMgr::Asset.AssetRem( Get()->GetType(), Get()->GetID() );
	*this = NULL;
}

}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
