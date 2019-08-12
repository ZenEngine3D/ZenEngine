#pragma once

#if ZEN_ENGINETOOL
namespace zeAss
{
	class PackageGroup : public zRefCounted
	{
	zenClassDeclare(PackageGroup, zRefCounted)
	public:		
																	PackageGroup	(const zWString& _zName, zenAss::zPackageGroupRef _Parent);
		virtual														~PackageGroup	();		
		
		zenInline const zWString&									GetName			()const;
		zenInline const zenAss::zPackageGroupRef&					GetParentGroup	()const;
		zenInline const zArrayDyn<zenAss::zPackageRef>&			GetPackages		()const;
		zenInline const zArrayDyn<zenAss::zPackageGroupRef>&	GetChildren		()const;
		zenInline zHash64											GetNameHashID	()const;
		zenInline const zWString&									GetStorageInfo	()const;
		zenInline bool												GetDirty		()const;
		zenInline bool												GetDirtyChild	()const;
		zUInt														GetPackageCount	()const;

		bool														IsParentOf		(const zenAss::zPackageGroupRef& _Child)const;
		bool														IsParentOf		(const zenAss::zPackageRef& _Child)const;

		void														SetParentGroup	(const zenAss::zPackageGroupRef& _rParent);
		void														SetName			(const zWString& _zName);
		zenInline void												SetStorageInfo	(const zWString& _zStorageInfo);
				
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
		zArrayDyn<zenAss::zPackageRef>							maPackage;		//!< Packages inside of it		
		zArrayDyn<zenAss::zPackageGroupRef>						maChildren;		//!< List of children group having this item as parent
		friend class ManagerAsset;
		friend class Package;		
	};
} //namespace zeAss

#endif

#include "zeAssPackageGroup.inl"
