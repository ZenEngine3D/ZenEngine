#pragma once
#ifndef __LibZenEngine_Asset_Package_h__
#define __LibZenEngine_Asset_Package_h__

#if AW_ENGINETOOL
namespace zen { namespace zeAss
{
	class Package
	{
	ZENClassDeclareNoParent(Package)
	public:
													Package			();
		virtual										~Package		();
		
		bool										Save			();
		inline zenHash32							GetID			(){return mID;}
		inline const zenArrayStatic<zenString>&		GetGroupAndName	(){return maGroup;}
		
		inline const zenString&						GetName			(){return *maGroup.Last();}		
//		bool										AssetAdd		(AssetItem* _pAsset);
//		void										AssetRemove		(AssetItem* _pAsset);
		AssetItem*									AssetGet		(AssetItem::enumType _eType, zenHash64 _hAssetName);
		inline zenMap<AssetItem*>::Key64&			AssetGet		(AssetItem::enumType _eType){return madAssetPerType[_eType];}
	
	protected:			
		bool										Load			(const CFil::Filename& _Filename, zenMap<AssetItem*>::Key64& _dAllAsset);
		void										Unload			();		

		zenHash32									mID;				//!< Package ID, taken from path		
		zenArrayStatic<zenString>					maGroup;			//!< Package belongs to a group hierarchy for easier finding of packages (last element is package name)
		
		zenHash32									mhGroupID;			//!< Group ID of this package
		CFil::Filename								mPackagePath;		//!< Full path to package file
		zenArrayStatic<zenMap<AssetItem*>::Key64>	madAssetPerType;	//!< Asset dictionary per Asset type
		friend class ManagerAsset;
	};

	void LoadGroupAndName(const char* _zName, const char* _zGroup, zenArrayStatic<zenString>& _aOutGroup );
}} //namespace zen { namespace zeAss

#endif
#endif