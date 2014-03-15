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
		
		bool										Save			();
		ZENInline zHash64							GetID			()const{return mID;}
		ZENInline const zArrayStatic<zString>&		GetGroupAndName	()const{return maGroup;}		
		ZENInline const zString&					GetName			()const{return *maGroup.Last();}		
		ZENInline const zenAss::zAssetItem&					GetAsset		(zenConst::eAssetType _eType, zHash64 _hAssetName){return madAssetPerType[_eType][_hAssetName];}
		ZENInline const zMap<zenAss::zAssetItem>::Key64&	GetAsset		(zenConst::eAssetType _eType){return madAssetPerType[_eType];}

	protected:			
		bool										Load			(const CFil::Filename& _Filename, zMap<zenAss::zAssetItem>::Key64& _dAllAsset);
		void										Unload			();		

		zHash64										mID;				//!< Package ID, taken from path		
		zArrayStatic<zString>						maGroup;			//!< Package belongs to a group hierarchy for easier finding of packages (last element is package name)
		zHash32										mhGroupID;			//!< Group ID of this package
		CFil::Filename								mPackagePath;		//!< Full path to package file
		zArrayStatic<zMap<zenAss::zAssetItem>::Key64>		madAssetPerType;	//!< Asset dictionary per Asset type
		friend class ManagerAsset;
	};

	void LoadGroupAndName(const char* _zName, const char* _zGroup, zArrayStatic<zString>& _aOutGroup );
}} //namespace zen { namespace zeAss

#endif
#endif