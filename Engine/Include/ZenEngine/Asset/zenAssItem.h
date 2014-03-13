#pragma once
#ifndef __zenEngine_Asset_Item_h__
#define __zenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss { class AssetItem; } }
namespace zen { namespace zenAss { class zPackage; } }

namespace zen { namespace zenAss 
{
	class zAssetItem : public zRefOwner<zeAss::AssetItem>
	{
	ZENClassDeclare(zAssetItem, zRefOwner<zeAss::AssetItem>)
	public:
										zAssetItem();
										zAssetItem(const zAssetItem& _Copy);
										zAssetItem(zeAss::AssetItem* _pAsset);
		zHash64							GetID()const;
		const zString&					GetName()const;
		const zArrayStatic<zString>&	GetGroupAndName()const;
		zHash32							GetGroupID()const;
		const zString&					GetDescription()const;
		zUInt							GetValueCount()const;
		zenConst::eAssetType			GetType()const;
		zPackage						GetPackage();
		zenAss::PropertyValue&			GetValue(zUInt _uValIndex);
		void							InitDefault();

		zAssetItem&						operator=(const zAssetItem& _Copy);		
	};

	zAssetItem							AssetCreate(zenConst::eAssetType _eAssetType);

	class zAssetTexture2D : public zAssetItem
	{
	ZENClassDeclare(zAssetTexture2D, zAssetItem)
	};

	

}} //namespace zen { namespace zenAss

#endif
#endif