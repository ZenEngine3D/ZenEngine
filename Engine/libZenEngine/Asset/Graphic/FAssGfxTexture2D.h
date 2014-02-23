#pragma once
#if !defined(__LibAsset_Graphics_Texture_h__) && AW_ENGINETOOL
#define __LibAsset_Graphics_Texture_h__

namespace FAss
{
	class GfxTexture2D : public FAss::AssetItem
	{
	AWClassDeclare(GfxTexture2D, FAss::AssetItem)
	public:
		virtual FAss::AssetItem::enumType	GetType()const{return keType_Texture2D;}
		virtual FAss::PropertyArray&		GetPropertyDef() const;		
		virtual zenUInt						GetPropertyDefIndex(zenHash32 _hPropertyName)const;	
	protected:
		mutable zenArrayStatic<zenU32>			maIcon; //!< Store icon data
	};	
}

#endif
