#pragma once
#ifndef __LibZenEngine_Asset_Gfx_Texture_h__
#define __LibZenEngine_Asset_Gfx_Texture_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
	class GfxTexture2D : public AssetItem
	{
	ZENClassDeclare(GfxTexture2D, AssetItem)
	public:
		virtual zenConst::eAssetType			GetType()const{return zenConst::keAssType_Texture2D;}
		virtual PropertyArray&					GetPropertyDef() const;		
		virtual zenUInt							GetPropertyDefIndex(zenHash32 _hPropertyName)const;	
		virtual const zenAss::PropertyArray&	GetProperties()const{ static zenAss::PropertyArray test(0); return test;};
	protected:
		mutable zenArrayStatic<zenU32>	maIcon; //!< Store icon data
	};	

}} //namespace zen { namespace zeAss

#endif
#endif
