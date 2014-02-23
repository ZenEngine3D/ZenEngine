#pragma once
#ifndef __LibEngine_Type_ResourceID_h__
#define __LibEngine_Type_ResourceID_h__

/*
namespace awengine { namespace awetype {

//=================================================================================================
//! @class		Resource
//-------------------------------------------------------------------------------------------------
//! @brief				
//=================================================================================================
class ResourceID
{
AWClassDeclareNoParent(ResourceID)
public:
	typedef awHash32	NameHash;
	
	enum ePlatformType
	{
		kePlatformType_OS,
		kePlatformType_GFX,
		kePlatformType__Count,
	};
									ResourceID():mhName(""), meType(awconst::keResType__Invalid), mePlatform(awconst::keResPlatform__Invalid), meSource(awconst::keResSource_Runtime){}
									ResourceID(NameHash _hName, awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								
						
	inline const NameHash&			Name()const							{ return mhName; }
	inline awconst::eResType		Type()const							{ return static_cast<awconst::eResType>(meType); }
	inline awconst::eResPlatform	Platform()const						{ return static_cast<awconst::eResPlatform>(mePlatform); } 
	inline awconst::eResSource		Source()const						{ return static_cast<awconst::eResSource>(meSource); }
	inline awU64						HashID()const						{ return muHashID; }
	inline void						SetName(NameHash _hName)			{ mhName = _hName; }
	inline void						SetType(awconst::eResType _eResType)			{ meType = _eResType; }
	inline void						SetPlatform(awconst::eResPlatform _ePlatform)	{ mePlatform = _ePlatform; }
	inline void						SetSource(awconst::eResSource _eSource)			{ meSource = _eSource; }
	inline bool						IsRuntime()const					{ return meSource==awconst::keResSource_Runtime; }
	inline bool						IsExport()const						{ return meSource==awconst::keResSource_Offline; }
	inline bool						IsLoaded()const						{ return meSource==awconst::keResSource_Loaded; }
	inline bool						IsValid()const						{ return meType < awconst::keResType__Count; }
	inline bool						operator==(const ResourceID& _Compare)const{return muHashID == _Compare.muHashID;};
	inline bool						operator!=(const ResourceID& _Compare)const{return muHashID != _Compare.muHashID;};
	ResourceID&						operator=(const ResourceID& _Copy){muHashID = _Copy.muHashID; return *this;}
	ResourceID&						operator=(const awResourceID& _Copy){muHashID = _Copy; return *this;}

protected:
	union
	{
		struct 
		{			
			NameHash	mhName;		//!< Hashed name of the resource
			awU16			meType;		//!< Resource type (eType)
			awU8			mePlatform;	//!< Platform this should be used on
			awU8			meSource;	//!< Source of the resource (eSource)			
		};
		awResourceID	muHashID;
	};
};


}}  //namespace awengine, type 
*/
#endif