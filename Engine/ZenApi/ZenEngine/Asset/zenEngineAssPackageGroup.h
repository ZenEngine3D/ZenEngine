#pragma once

#if ZEN_ENGINETOOL

namespace zeAss { class PackageGroup; }	//Forward declare

namespace zen { namespace zenAss 
{
	class zPackageGroupRef : public zEngineRef<zeAss::PackageGroup>
	{
	zenClassDeclare(zPackageGroupRef, zEngineRef)
	public:	
													zPackageGroupRef();
													zPackageGroupRef(const zPackageGroupRef& _Copy);
													zPackageGroupRef(zeAss::PackageGroup* _pGroup);

		const zWString&								GetName()const;		
		const zPackageGroupRef&						GetParentGroup()const;
		const zArrayDyn<zenAss::zPackageRef>&	GetPackages()const;
		const zArrayDyn<zPackageGroupRef>&		GetChildren()const;
		zHash64										GetNameHashID()const;
		zUInt										GetPackageCount()const;
		bool										GetDirty()const;
		bool										GetDirtyChild()const;
		
		bool										IsParentOf(const zenAss::zPackageGroupRef& _Child)const;
		bool										IsParentOf(const zenAss::zPackageRef& _Child)const;

		void										SetParentGroup(const zPackageGroupRef& _rParent);
		void										SetName(const zWString& _zName);
		
		void										Delete();
		//void										Save();

		//-----------------------------------------------------------------------------------------
		static const zPackageGroupRef&				sGetRoot();
		static zPackageGroupRef						sCreate	(const zWString& _zName, const zPackageGroupRef& _rParent=sGetRoot());		
		//-----------------------------------------------------------------------------------------
	};

	//! @todo Asset: Find proper place
	bool SaveAll();
}} //namespace zen { namespace zenAss

#endif
