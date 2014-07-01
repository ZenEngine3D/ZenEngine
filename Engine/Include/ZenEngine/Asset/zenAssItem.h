#pragma once
#ifndef __zenEngine_Asset_Item_h__
#define __zenEngine_Asset_Item_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss { class Asset; } }

namespace zen { namespace zenAss 
{
	class zAssetItem : public zEngineRef<zeAss::Asset>
	{
	ZENClassDeclare(zAssetItem, zEngineRef)
	public:
										zAssetItem();
										zAssetItem(const zAssetItem& _Copy);
										zAssetItem(zeAss::Asset* _pAsset);
		const zAssetID&					GetID()const;
		const zString&					GetName()const;
		const zArrayStatic<zString>&	GetGroupAndName()const;
		const zString&					GetDescription()const;
		zUInt							GetValueCount()const;
		zenConst::eAssetType			GetType()const;
		class zPackage					GetPackage();
		zenAss::PropertyValueRef		GetValue(zUInt _uValIndex);
		void							InitDefault();
		
		void							SetPackage(zPackage _rPackage);
		void							SetName(const char* _zName);

		void							Delete();
		zAssetItem&						operator=(const zAssetItem& _Copy);		
	};
	
	zAssetItem							AssetCreate			(zenConst::eAssetType _eAssetType, zPackage& _rPackage, const char* _zGroup);
	const char*							AssetTypeToString	(zenConst::eAssetType _ePropertyType);
	zenConst::eAssetType				AssetNameToType		(zHash32 _hAssetName);
	const zAssetItem&					AssetGet			(const zAssetID& _uAssetID); //! @todo Asset: Move to an asset manager at api level?

	typedef zArraySparse<zenAss::zAssetItem>::Key32 zArrayAsset;
	

}} //namespace zen { namespace zenAss

#endif
#endif