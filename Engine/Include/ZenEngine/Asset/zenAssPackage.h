#pragma once
#ifndef __zenEngine_Asset_Package_h__
#define __zenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss  { class Package; } }	//Forward declare

namespace zen { namespace zenAss 
{
	class zPackage : public zRefOwner<zeAss::Package>
	{
	ZENClassDeclare(zPackage, zRefOwner)
	public:
											zPackage();
											zPackage(const zPackage& _Copy);
											zPackage(zeAss::Package* _pAsset);
		zPackage&							operator=(const zPackage& _Copy);		

		zHash64								GetID			()const;
		const zArrayStatic<zString>&		GetGroupAndName	()const;
		const zString&						GetName			()const;
		const zMap<zAssetItem>::Key64&		GetAssets		(zenConst::eAssetType _eType);
	};

	const zPackage&							GetPackage		( zHash64 _hPackageID );
	const zMap<zPackage>::Key64&			GetPackages		();

}} //namespace zen { namespace zenAss

#endif
#endif
