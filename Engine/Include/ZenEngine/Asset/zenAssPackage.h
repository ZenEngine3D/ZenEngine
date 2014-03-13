#pragma once
#ifndef __zenEngine_Asset_Package_h__
#define __zenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss  { class Package; } }

namespace zen { namespace zenAss 
{
	class zPackage : public zRefOwner<zeAss::Package>
	{
	ZENClassDeclare(zPackage, zRefOwner<zeAss::Package>)
	public:
											zPackage();
											zPackage(const zPackage& _Copy);
											zPackage(zeAss::Package* _pAsset);

		zHash64								GetID			()const;
		const zArrayStatic<zString>&		GetGroupAndName	()const;
		const zString&						GetName			()const;
		const zMap<zAssetItem>::Key64&		GetAsset		(zenConst::eAssetType _eType);
		//AssetItem*						AssetGet		(zenConst::eAssetType _eType, zHash64 _hAssetName);
		
		zPackage&							operator=(const zPackage& _Copy);		
	};


}} //namespace zen { namespace zenAss

#endif
#endif