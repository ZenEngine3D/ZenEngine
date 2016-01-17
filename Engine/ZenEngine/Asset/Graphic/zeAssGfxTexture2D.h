#pragma once
#ifndef __LibZenEngine_Asset_Gfx_Texture_h__
#define __LibZenEngine_Asset_Gfx_Texture_h__

#if ZEN_ENGINETOOL

namespace zeAss
{
	class GfxTexture2D : public AssetTyped<zenConst::keAssType_Texture2D>
	{
	ZENClassDeclare(GfxTexture2D, Asset)
	public:
		virtual const zenAss::PropertyDefArray&	GetProperties()const;
	};	

} //namespace zeAss

#endif
#endif
