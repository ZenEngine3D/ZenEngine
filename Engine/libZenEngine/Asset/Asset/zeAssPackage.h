#pragma once
#ifndef __LibZenEngine_Asset_Package_h__
#define __LibZenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL
namespace zen { namespace zeAss
{
	class Package : public zRefCountedAutoDel
	{
	ZENClassDeclare(Package, zRefCountedAutoDel)
	public:		
													Package			();
		virtual										~Package		();
				
		ZENInline zU32								GetID			()const;
		ZENInline const zArrayStatic<zString>&		GetGroupAndName	()const;
		ZENInline const zString&					GetName			()const;
		ZENInline const zString&					GetStorageName	()const;

		ZENInline const zenAss::zAssetItem&			GetAsset		(zenConst::eAssetType _eType, zU32 _uAssetID);
		ZENInline const zenAss::zArrayAsset&		GetAsset		(zenConst::eAssetType _eType);
		
		void										Unload			();		//! @todo Asset: Useful?
		bool										Save			();

		bool										Init			(zU32 _uID, const char* _zName, const char* _zGroup, const char* _zStorageName, zU32 _uEngineVer);
		ZENInline void								AssetAdd		(zeAss::Asset& _Asset);
		ZENInline void								AssetRem		(zeAss::Asset& _Asset);
		
	protected:		
		zU32										mID;				//!< Package ID, taken from path		
		zU32										muEngineVersion;	//!< Engine version used to save the package
		zArrayStatic<zString>						maGroup;			//!< Package belongs to a group hierarchy for easier finding of packages (last element is package name)
		zString										mzStorageName;		//!< Where to save this package(filename, database entry id, etc....)
		zArrayStatic<zenAss::zArrayAsset>			madAssetPerType;	//!< Asset array per Asset type

	};
}} //namespace zen { namespace zeAss

#endif

#include "zeAssPackage.inl"

#endif
