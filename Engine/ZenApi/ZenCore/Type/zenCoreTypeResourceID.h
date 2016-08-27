#pragma once
#ifndef __zenCore_Type_ResourceID_h__
#define __zenCore_Type_ResourceID_h__

namespace zen { namespace zenType {

	class zResID
	{
	zenClassDeclareNoParent(zResID)
	public:
		typedef zHash32	NameHash; //! @todo system hash64 for collision safety?

		enum ePlatformType
		{
			kePlatformType_OS,
			kePlatformType_GFX,
			kePlatformType__Count,
		};
		zResID():mhName(""), meType(zenConst::keResType__Invalid), mePlatform(zenConst::keResPlatform__Invalid), meSource(zenConst::keResSource_Runtime){}
		zResID(NameHash _hName, zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								

		ZENInline const NameHash&			GetName()const									{ return mhName; }
		ZENInline zenConst::eResType		GetType()const									{ return static_cast<zenConst::eResType>(meType); }
		ZENInline zenConst::eResPlatform	GetPlatform()const								{ return static_cast<zenConst::eResPlatform>(mePlatform); } 
		ZENInline zenConst::eResSource		GetSource()const								{ return static_cast<zenConst::eResSource>(meSource); }
		ZENInline zU64						GetHashID()const								{ return muHashID; }
		ZENInline void						SetName(NameHash _hName)						{ mhName = _hName; }
		ZENInline void						SetType(zenConst::eResType _eResType)			{ meType = _eResType; }
		ZENInline void						SetPlatform(zenConst::eResPlatform _ePlatform)	{ mePlatform = _ePlatform; }
		ZENInline void						SetSource(zenConst::eResSource _eSource)		{ meSource = _eSource; }
		ZENInline bool						IsRuntime()const								{ return meSource==zenConst::keResSource_Runtime; }
		ZENInline bool						IsExport()const									{ return meSource==zenConst::keResSource_Offline; }
		ZENInline bool						IsLoaded()const									{ return meSource==zenConst::keResSource_Loaded; }
		ZENInline bool						IsValid()const									{ return meType < zenConst::keResType__Count; }
		ZENInline bool						operator==(const zResID& _Compare)const			{return muHashID == _Compare.muHashID;};
		ZENInline bool						operator!=(const zResID& _Compare)const			{return muHashID != _Compare.muHashID;};
		const zResID&						operator=(const zResID& _Copy)					{muHashID = _Copy.muHashID; return *this;}

	protected:
		union
		{
			//! @todo optim Use a Handle system for loaded res, on top for faster access ?
			struct 
			{			
				NameHash	mhName;		//!< Hashed name of the resource
				zU16		meType;		//!< Resource type (eType)
				zU8			mePlatform;	//!< Platform this should be used on
				zU8			meSource;	//!< Source of the resource (eSource)			
			};
			zU64 muHashID;
		};
	};
		

}}  //namespace zen, Type 

#endif