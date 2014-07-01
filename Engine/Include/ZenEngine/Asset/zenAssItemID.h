#pragma once
#ifndef __zenEngine_Asset_ItemID_h__
#define __zenEngine_Asset_ItemID_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zenAss 
{
	
	struct zAssetID
	{
		ZENInline				zAssetID():meType(zenConst::keAssType__Invalid),muIndex(0){}
		ZENInline				zAssetID(zenConst::eAssetType _eType, zU32 _uIndex):meType(_eType), muIndex(_uIndex){};
		ZENInline 				zAssetID(const zAssetID& _Copy):meType(_Copy.meType), muIndex(_Copy.muIndex){}
		ZENInline zAssetID&		operator=(const zAssetID& _Copy){meType = _Copy.meType; muIndex = _Copy.muIndex; return *this;}
		ZENInline 				operator const zU64() const{return *reinterpret_cast<const zU64*>(this);}
		zenConst::eAssetType	meType;
		zU32					muIndex;
	};

}} //namespace zen { namespace zenAss

#endif
#endif