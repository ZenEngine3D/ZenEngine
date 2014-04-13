#pragma once
#ifndef __zenEngine_Asset_Item_h__
#define __zenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss { class Asset; } }
namespace zen { namespace zenAss { class zPackage; } }

namespace zen { namespace zenAss 
{
	class zAssetItem : public zRefOwner<zeAss::Asset>
	{
	ZENClassDeclare(zAssetItem, zRefOwner)
	public:
										zAssetItem();
										zAssetItem(const zAssetItem& _Copy);
										zAssetItem(zeAss::Asset* _pAsset);
		zU32							GetID()const;
		const zString&					GetName()const;
		const zArrayStatic<zString>&	GetGroupAndName()const;
		const zString&					GetDescription()const;
		zUInt							GetValueCount()const;
		zenConst::eAssetType			GetType()const;
		zPackage						GetPackage();
		zenAss::PropertyValue&			GetValue(zUInt _uValIndex);
		void							InitDefault();

		void							Delete();
		zAssetItem&						operator=(const zAssetItem& _Copy);		
	};
	
	typedef zArraySparse<zenAss::zAssetItem>::Key32 zArrayAsset;
	zAssetItem							AssetCreate			(zenConst::eAssetType _eAssetType);
	const char*							AssetTypeToString	(zenConst::eAssetType _ePropertyType);
	zenConst::eAssetType				AssetNameToType		(zHash32 _hAssetName);
	const zAssetItem&					AssetGet			(zenConst::eAssetType _eAssetType, zU32 _uAssetID); //! @todo Asset: Move to an asset manager at api level?

	class zAssetTexture2D : public zAssetItem
	{
	ZENClassDeclare(zAssetTexture2D, zAssetItem)
	};

	

}} //namespace zen { namespace zenAss

#endif
#endif