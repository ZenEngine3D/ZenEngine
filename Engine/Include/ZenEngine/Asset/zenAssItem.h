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
		struct ID
		{
			zenConst::eAssetType	meType;
			zU32					muIndex;
			ZENInline				ID(zenConst::eAssetType _eType, zU32 _uIndex):meType(_eType), muIndex(_uIndex){};
			ZENInline 				operator const zU64() const{return *reinterpret_cast<const zU64*>(this);}
		};
	
	public:
										zAssetItem();
										zAssetItem(const zAssetItem& _Copy);
										zAssetItem(zeAss::Asset* _pAsset);
		ID								GetID()const;
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
	
	zAssetItem							AssetCreate			(zenConst::eAssetType _eAssetType);
	const char*							AssetTypeToString	(zenConst::eAssetType _ePropertyType);
	zenConst::eAssetType				AssetNameToType		(zHash32 _hAssetName);
	const zAssetItem&					AssetGet			(zAssetItem::ID _uAssetID); //! @todo Asset: Move to an asset manager at api level?

	typedef zArraySparse<zenAss::zAssetItem>::Key32 zArrayAsset;

	class zAssetTexture2D : public zAssetItem
	{
	ZENClassDeclare(zAssetTexture2D, zAssetItem)
	};

	

}} //namespace zen { namespace zenAss

#endif
#endif