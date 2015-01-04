#pragma once
#ifndef __zenEngine_Asset_ItemID_h__
#define __zenEngine_Asset_ItemID_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{	
	struct zAssetID
	{
		ZENInline 				zAssetID();
		ZENInline  				zAssetID(zenConst::eAssetType _eType, zU32 _uIndex);
		ZENInline  				zAssetID(const zAssetID& _Copy);
		ZENInline zAssetID&		operator=(const zAssetID& _Copy);
		ZENInline bool			operator==(const zAssetID& _Copy)const;
		ZENInline bool			operator!=(const zAssetID& _Copy)const;
		ZENInline bool			IsValid()const;
		ZENInline zU64			ToUInt()const;
		zenConst::eAssetType	meType;
		zU32					muIndex;
	};
}} //namespace zen { namespace zenAss

#include "zenAssItemID.inl"

#endif
#endif