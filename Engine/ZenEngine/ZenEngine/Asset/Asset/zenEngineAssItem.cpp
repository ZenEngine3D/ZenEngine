#include "zeEngine.h"

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss
{

const zStringHash32 sTypeDescription[]={
						zStringHash32("TestProperty"),	//keType_TestProperty,
						zStringHash32("Texture2D"),		//keType_Texture2D,
						zStringHash32("Mesh"),			//keType_Mesh,
						};

zAssetItemRef AssetCreate( zenConst::eAssetType _eAssetType, zPackageRef& _rPackage )
{
	ZENAssert( _rPackage.IsValid() );	
	zeAss::Asset* pNewItem	= zeAss::Asset::CreateItem(_eAssetType);
	if( pNewItem )
	{
		char zName[256];
		zAssetID assetID = zeMgr::Asset.GetAssetNextID(_eAssetType);
		sprintf_s(zName, sizeof(zName), "%s_%06i", zenAss::AssetTypeToString(_eAssetType), assetID.muIndex );
		pNewItem->Init( assetID, zName, _rPackage );
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

const zAssetItemRef& AssetGet( const zAssetID& _uAssetID )
{
	return zeMgr::Asset.AssetGet(_uAssetID.meType, _uAssetID.muIndex);
}

zAssetItemRef::zAssetItemRef()
:Super()
{
}

zAssetItemRef::zAssetItemRef(const zAssetItemRef& _Copy)
: Super(_Copy)
{
}

zAssetItemRef::zAssetItemRef(zeAss::Asset* _pAsset)
: Super(_pAsset)
{
}

zAssetItemRef& zAssetItemRef::operator=(const zAssetItemRef& _Copy)
{
	Super::operator=(_Copy);
	return *this;
}

const zAssetID& zAssetItemRef::GetID()const								
{ 
	return GetSafe()->GetID();
}

zU64 zAssetItemRef::GetIDUInt()const
{
	return GetSafe()->GetID().ToUInt();
}

zenConst::eAssetType zAssetItemRef::GetType()const
{
	return GetSafe()->GetType(); 
}

const zString& zAssetItemRef::GetName()const						
{ 
	return GetSafe()->GetName(); 
}

const zString& zAssetItemRef::GetDescription()const				
{ 
	return GetSafe()->GetDescription(); 
}

zUInt zAssetItemRef::GetValueCount()const							
{ 
	return GetSafe()->GetValueCount(); 
}

zenAss::PropertyValueRef zAssetItemRef::GetValue(zUInt _uValIndex)	
{ 
	return GetSafe()->GetValue(_uValIndex); 
}

void zAssetItemRef::InitDefault()									
{ 
	return GetSafe()->InitDefault(); 
}

void zAssetItemRef::Delete()
{
	zeMgr::Asset.AssetDelete( *this );
	*this = NULL;
}

zPackageRef zAssetItemRef::GetPackage()
{
	return GetSafe()->GetPackage();
}

void zAssetItemRef::SetPackage(zPackageRef& _rPackage)
{
	GetSafe()->SetPackage( _rPackage );
}

void zAssetItemRef::SetName(const char* _zName)
{
	GetSafe()->SetName(_zName);
}

zenSig::zSignal1<zenAss::PropertyValueRef>& zAssetItemRef::GetSignalUpdatedProperty()
{
	return GetSafe()->GetSignalUpdatedProperty();
}


}} //namespace zen { namespace zenAss

#endif //ZEN_ENGINETOOL
