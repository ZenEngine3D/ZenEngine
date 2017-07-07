#pragma once

#if ZEN_ENGINETOOL

namespace zeAss
{
	class GfxTexture2D : public AssetTyped<zenConst::keAssType_Texture2D>
	{
	zenClassDeclare(GfxTexture2D, Asset)
	public:
		virtual const zenAss::PropertyDefArray&	GetProperties()const;
	};	

} //namespace zeAss

#endif
