#include "libZenEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

const zStringHash32 sTypeDescription[]={
						zStringHash32("TestProperty"),	//keType_TestProperty,
						zStringHash32("Texture2D"),		//keType_Texture2D,
						zStringHash32("Mesh"),			//keType_Mesh,
						};

zAssetItem AssetCreate( zenConst::eAssetType _eAssetType, zPackage& _rPackage, const char* _zGroup )
{
	ZENAssert( _rPackage.IsValid() );
	zeAss::Asset* pNewItem = zeAss::Asset::CreateItem(_eAssetType);
	if( pNewItem )
	{
		char zName[256];
		zAssetID id = zeMgr::Asset.GetAssetNextID(_eAssetType);
		sprintf_s(zName, sizeof(zName), "New %s %i", zenAss::AssetTypeToString(_eAssetType), id.muIndex );
		pNewItem->Init( id, zName, _zGroup, *_rPackage.Get() );
		return pNewItem;
	}
	return NULL;
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

const zAssetItem& AssetGet( const zAssetID& _uAssetID )
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

const zAssetID& zAssetItem::GetID()const								
{ 
	ZENAssert(IsValid());
	return Get()->GetID();
}
zenConst::eAssetType zAssetItem::GetType()const
{
	ZENAssert(IsValid());
	return Get()->GetType(); 
}
const zString& zAssetItem::GetName()const						
{ 
	ZENAssert(IsValid());
	return Get()->GetName(); 
}
const zArrayStatic<zString>& zAssetItem::GetGroupAndName()const 
{ 
	ZENAssert(IsValid());
	return Get()->GetGroupAndName(); 
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
	return Get()->GetDescription(); 
}
zUInt zAssetItem::GetValueCount()const							
{ 
	ZENAssert(IsValid());
	return Get()->GetValueCount(); 
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
	zeMgr::Asset.AssetRem( Get()->GetID() );
	*this = NULL;
}

zPackage zAssetItem::GetPackage()
{
	ZENAssert(IsValid());
	return Get()->GetPackage();
}

void zAssetItem::SetPackage(zPackage _rPackage)
{
	ZENAssert(IsValid());
	Get()->SetPackage( _rPackage.Get() );
}
void zAssetItem::SetName(const char* _zName)
{
	ZENAssert(IsValid());
	Get()->SetName(_zName);
}
}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
