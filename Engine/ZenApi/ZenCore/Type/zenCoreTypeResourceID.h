#pragma once

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
		zResID():mhName(""), meType(keResType__Invalid), mePlatform(kePlatform__Invalid), meSource(keResSource_Runtime){}
		zResID(NameHash _hName, ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								

		zenInline const NameHash&	GetName()const							{ return mhName; }
		zenInline eResType			GetType()const							{ return meType; }
		zenInline ePlatform			GetPlatform()const						{ return mePlatform; } 
		zenInline eResSource		GetSource()const						{ return meSource; }
		zenInline zU64				GetHashID()const						{ return muHashID; }
		zenInline void				SetName(NameHash _hName)				{ mhName = _hName; }
		zenInline void				SetType(zenConst::eResType _eResType)	{ meType = _eResType; }
		zenInline void				SetPlatform(ePlatform _ePlatform)		{ mePlatform = _ePlatform; }
		zenInline void				SetSource(zenConst::eResSource _eSource){ meSource = _eSource; }
		zenInline bool				IsRuntime()const						{ return meSource==keResSource_Runtime; }
		zenInline bool				IsExport()const							{ return meSource==keResSource_Offline; }
		zenInline bool				IsLoaded()const							{ return meSource==keResSource_Loaded; }
		zenInline bool				IsValid()const							{ return meType < keResType__Count; }
		zenInline bool				operator==(const zResID& _Compare)const	{return muHashID == _Compare.muHashID;};
		zenInline bool				operator!=(const zResID& _Compare)const	{return muHashID != _Compare.muHashID;};
		const zResID&				operator=(const zResID& _Copy)			{muHashID = _Copy.muHashID; return *this;}
		const zResID&				operator=(zU64 _Copy)			{muHashID = _Copy; zenAssert(meType<keResType__Count && meSource<keResSource__Count && mePlatform<kePlatform__Count); return *this;}

	protected:
		union
		{
			//! @todo optim Use a Handle system for loaded res, on top for faster access ?
			struct 
			{			
				NameHash	mhName;		//!< Hashed name of the resource
				eResType	meType;		//!< Resource type (eType)
				ePlatform	mePlatform;	//!< Platform this should be used on
				eResSource	meSource;	//!< Source of the resource (eSource)			
			};
			zU64 muHashID;
		};
	};
		

}}  //namespace zen, Type 
