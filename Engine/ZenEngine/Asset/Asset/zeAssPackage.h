#pragma once
#ifndef __LibZenEngine_Asset_Package_h__
#define __LibZenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL
namespace zeAss
{
	class Package : public zRefCounted
	{
	ZENClassDeclare(Package, zRefCounted)
	public:		
													Package			();
		virtual										~Package		();
				
		ZENInline zU32								GetID			()const;
		ZENInline const zWString&					GetName			()const;
		ZENInline const zenAss::zPackageGroupRef&	GetParentGroup	()const;
		ZENInline const zWString&					GetStorageInfo	()const;

		void										SetName			(const zWString& _zName);		
		void										SetStorageInfo	(const zWString& _zStorageInfo);
		void										SetParentGroup	(const zenAss::zPackageGroupRef& _rParent);		
		void										SetDirty		();

		ZENInline const zenAss::zAssetItemRef&		GetAsset		(zenConst::eAssetType _eType, zU32 _uAssetID)const;
		ZENInline const zenAss::zArrayAsset&		GetAsset		(zenConst::eAssetType _eType)const;
		
		void										Unload			();		//! @todo Asset: Useful?
		
		void										Delete			();		

		bool										Init			(zU32 _uID, const zWString& _zName, const zenAss::zPackageGroupRef& _rGroupParent, zU32 _uEngineVer);
		ZENInline void								AssetAdd		(const zenAss::zAssetItemRef& _rAsset);
		ZENInline void								AssetRem		(const zenAss::zAssetItemRef& _rAsset);
		
		ZENInline bool								GetDirty			()const;

	protected:				
		bool										Save			(class AssetSerializer &_Serializer, bool _bDirtyParent);
		zU32										mID;				//!< Package ID, taken from path		
		zU32										muEngineVersion;	//!< Engine version used to save the package
		zWString									mzName;				//!< Package name
		zenAss::zPackageGroupRef					mrGroupParent;		//!< Group of this package
		zWString									mzStorageInfo;		//!< Where to save this package(filename, database entry id, etc....)
		zArrayStatic<zenAss::zArrayAsset>			madAssetPerType;	//!< Asset array per Asset type
		bool										mbDirty;
		friend class PackageGroup;
	};
} //namespace zeAss

#endif

#include "zeAssPackage.inl"

#endif
