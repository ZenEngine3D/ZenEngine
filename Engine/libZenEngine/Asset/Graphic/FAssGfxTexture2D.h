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
		virtual awUInt						GetPropertyDefIndex(awHash32 _hPropertyName)const;	
	protected:
		mutable awArrayStatic<awU32>			maIcon; //!< Store icon data
	};	
}

#endif
