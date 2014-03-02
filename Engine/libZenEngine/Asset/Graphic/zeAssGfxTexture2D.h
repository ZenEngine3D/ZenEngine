#pragma once
#ifndef __LibZenEngine_Asset_Gfx_Texture_h__
#define __LibZenEngine_Asset_Gfx_Texture_h__

#if AW_ENGINETOOL

namespace zen { namespace zeAss
{
	class GfxTexture2D : public AssetItem
	{
	ZENClassDeclare(GfxTexture2D, AssetItem)
	public:
		virtual AssetItem::enumType		GetType()const{return keType_Texture2D;}
		virtual PropertyArray&			GetPropertyDef() const;		
		virtual zenUInt					GetPropertyDefIndex(zenHash32 _hPropertyName)const;	
	protected:
		mutable zenArrayStatic<zenU32>	maIcon; //!< Store icon data
	};	
}} //namespace zen { namespace zeAss

#endif
#endif
