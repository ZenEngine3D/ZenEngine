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
		virtual												~Package		();
				
		ZENInline zHash64									GetID			()const;
		ZENInline const zArrayStatic<zString>&				GetGroupAndName	()const;
		ZENInline const zString&							GetName			()const;
		ZENInline const zenAss::zAssetItem&					GetAsset		(zenConst::eAssetType _eType, zHash64 _hAssetName);
		ZENInline const zMap<zenAss::zAssetItem>::Key64&	GetAsset		(zenConst::eAssetType _eType);
		
		void												Unload			();		//! @todo Asset: Usefull?
		bool												Load			(const CFil::Filename& _Filename);
		bool												Save			(const CFil::Filename& _Filename);
		bool												Save			();

		ZENInline void										Init			(const char* _zGroup, const CFil::Filename& _Filename );
		ZENInline void										AssetAdd		(zeAss::AssetItem& _Asset);
		ZENInline void										AssetRem		(zeAss::AssetItem& _Asset);
	protected:		
		zHash64												mID;				//!< Package ID, taken from path		
		zArrayStatic<zString>								maGroup;			//!< Package belongs to a group hierarchy for easier finding of packages (last element is package name)
		zHash32												mhGroupID;			//!< Group ID of this package
		CFil::Filename										mFilename;			//!< Full path to package file
		zArrayStatic<zMap<zenAss::zAssetItem>::Key64>		madAssetPerType;	//!< Asset dictionary per Asset type
		friend class ManagerAsset;
	};

	void ParseGroupAndName(const char* _zName, const char* _zGroup, zArrayStatic<zString>& _aOutGroup );
}} //namespace zen { namespace zeAss

#endif

#include "zeAssPackage.inl"

#endif
