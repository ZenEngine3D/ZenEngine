#pragma once
#if !defined(__LibAsset_Asset_Package_h__) && AW_ENGINETOOL
#define __LibAsset_Asset_Package_h__

namespace FAss
{
	class Package
	{
	AWClassDeclareNoParent(Package)
	public:
												Package			();
		virtual									~Package		();
		
		bool									Save			();
		inline awHash32							GetID			(){return mID;}
		inline const awArrayStatic<awString>&		GetGroupAndName	(){return maGroup;}
		
		inline const awString&			GetName			(){return *maGroup.Last();}		
//		bool									AssetAdd		(AssetItem* _pAsset);
//		void									AssetRemove		(AssetItem* _pAsset);
		AssetItem*								AssetGet		(AssetItem::enumType _eType, awHash64 _hAssetName);
		inline Hashmap<AssetItem*>::Key64&		AssetGet		(AssetItem::enumType _eType){return madAssetPerType[_eType];}
	
	protected:			
		bool									Load			(const CFil::Filename& _Filename, Hashmap<AssetItem*>::Key64& _dAllAsset);
		void									Unload			();		

		awHash32									mID;				//!< Package ID, taken from path		
		awArrayStatic<awString>						maGroup;			//!< Package belongs to a group hierarchy for easier finding of packages (last element is package name)
		
		awHash32									mhGroupID;			//!< Group ID of this package
		CFil::Filename							mPackagePath;		//!< Full path to package file
		awArrayStatic<Hashmap<AssetItem*>::Key64>	madAssetPerType;	//!< Asset dictionary per Asset type
		friend class ManagerAsset;
	};

	void LoadGroupAndName(const char* _zName, const char* _zGroup, awArrayStatic<awString>& _aOutGroup );
}

#endif