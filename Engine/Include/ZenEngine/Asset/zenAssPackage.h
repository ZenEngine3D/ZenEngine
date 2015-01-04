#pragma once
#ifndef __zenEngine_Asset_Package_h__
#define __zenEngine_Asset_Package_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss  { class Package; } }	//Forward declare

namespace zen { namespace zenAss 
{
	typedef zArraySparse<zenAss::zPackageRef>::Key32 zArrayPackage;

	class zPackageRef : public zEngineRef<zeAss::Package>
	{
	ZENClassDeclare(zPackageRef, zEngineRef)
	public:		
											zPackageRef();
											zPackageRef(const zPackageRef& _Copy);
											zPackageRef(zeAss::Package* _pPackage);

		zU32								GetID			()const;
		const zWString&						GetName			()const;
		const zArrayAsset&					GetAssets		(zenConst::eAssetType _eType)const;
		const class zPackageGroupRef&		GetParentGroup	()const;
		bool								GetDirty		()const;

		void								SetParentGroup	(const zPackageGroupRef& _rParent);
		void								SetName			(const zWString& _Name);
		void								SetDirty		();

		void								Delete			();

		//-----------------------------------------------------------------------------------------
		static const zPackageRef&			sGetPackage		(zU32 _PackageID);
		static const zArrayPackage&			sGetPackages	();
		static zPackageRef					sCreate			(const zWString& _zName, const zPackageGroupRef& _rParent);		
		//-----------------------------------------------------------------------------------------
	};

	
	
	
}} //namespace zen { namespace zenAss

#endif
#endif
