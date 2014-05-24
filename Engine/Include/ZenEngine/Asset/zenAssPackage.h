#pragma once
#ifndef __zenEngine_Asset_Package_h__
#define __zenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss  { class Package; } }	//Forward declare

namespace zen { namespace zenAss 
{
	class zPackage : public zEngineRef<zeAss::Package>
	{
	ZENClassDeclare(zPackage, zEngineRef)
	public:
		
											zPackage();
											zPackage(const zPackage& _Copy);
											zPackage(zeAss::Package* _pAsset);
		zPackage&							operator=(const zPackage& _Copy);		

		zU32								GetID			()const;
		const zArrayStatic<zString>&		GetGroupAndName	()const;
		const zString&						GetName			()const;
		const zArrayAsset&					GetAssets		(zenConst::eAssetType _eType);
	};

	typedef zArraySparse<zenAss::zPackage>::Key32 zArrayPackage;
	const zPackage&							GetPackage		( zU32 _hPackageID );
	const zArrayPackage&					GetPackages		();
}} //namespace zen { namespace zenAss

#endif
#endif
