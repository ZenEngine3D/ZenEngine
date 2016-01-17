#pragma once
#ifndef __LibZenEngine_Asset_PackageGroup_h__
#define __LibZenEngine_Asset_PackageGroup_h__

#if ZEN_ENGINETOOL
namespace zeAss
{
	class PackageGroup : public zRefCounted
	{
	ZENClassDeclare(PackageGroup, zRefCounted)
	public:		
																	PackageGroup	(const zWString& _zName, zenAss::zPackageGroupRef _Parent);
		virtual														~PackageGroup	();		
		
		ZENInline const zWString&									GetName			()const;
		ZENInline const zenAss::zPackageGroupRef&					GetParentGroup	()const;
		ZENInline const zArrayDynamic<zenAss::zPackageRef>&			GetPackages		()const;
		ZENInline const zArrayDynamic<zenAss::zPackageGroupRef>&	GetChildren		()const;
		ZENInline zHash64											GetNameHashID	()const;
		ZENInline const zWString&									GetStorageInfo	()const;
		ZENInline bool												GetDirty		()const;
		ZENInline bool												GetDirtyChild	()const;
		zUInt														GetPackageCount	()const;

		bool														IsParentOf		(const zenAss::zPackageGroupRef& _Child)const;
		bool														IsParentOf		(const zenAss::zPackageRef& _Child)const;

		void														SetParentGroup	(const zenAss::zPackageGroupRef& _rParent);
		void														SetName			(const zWString& _zName);
		ZENInline void												SetStorageInfo	(const zWString& _zStorageInfo);
				
		void														Delete			();

	protected:
		bool														Save			(class AssetSerializer &_Serializer, bool _bDirtyParent);

		bool														SaveInternal	();
		void														SetDirty		();
		void														SetDirtyChild	();

		zWString													mName;			//!< Group name
		zWString													mzStorageInfo;	//!< Where to save this group (filename, database entry id, etc....)
		zenAss::zPackageGroupRef									mrParent;		//!< Parent group (nil if Root element)		
		zHash64														mNameHashID;	//!< Unique ID generated from parent ID and this group name
		bool														mbDirty;
		bool														mbDirtyChild;
		zArrayDynamic<zenAss::zPackageRef>							maPackage;		//!< Packages inside of it		
		zArrayDynamic<zenAss::zPackageGroupRef>						maChildren;		//!< List of children group having this item as parent
		friend class ManagerAsset;
		friend class Package;		
	};
} //namespace zeAss

#endif

#include "zeAssPackageGroup.inl"


#endif