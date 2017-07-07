#pragma once

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{	
	struct zAssetID
	{
		zenInline 				zAssetID();
		zenInline  				zAssetID(zenConst::eAssetType _eType, zU32 _uIndex);
		zenInline  				zAssetID(const zAssetID& _Copy);
		zenInline zAssetID&		operator=(const zAssetID& _Copy);
		zenInline bool			operator==(const zAssetID& _Copy)const;
		zenInline bool			operator!=(const zAssetID& _Copy)const;
		zenInline bool			IsValid()const;
		zenInline zU64			ToUInt()const;
		zenConst::eAssetType	meType;
		zU32					muIndex;
	};
}} //namespace zen { namespace zenAss

#include "zenEngineAssItemID.inl"
#endif
