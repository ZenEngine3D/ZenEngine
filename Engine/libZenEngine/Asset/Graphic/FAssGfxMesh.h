#pragma once
#if !defined(__LibAsset_Graphics_Mesh_h__) && AW_ENGINETOOL
#define __LibAsset_Graphics_Mesh_h__

namespace FAss
{
	class GfxMesh : public FAss::AssetItem
	{
	ZENClassDeclare(GfxMesh, FAss::AssetItem)
	public:
		virtual FAss::AssetItem::enumType	GetType()const{return keType_Mesh;}
		virtual FAss::PropertyArray&		GetPropertyDef() const;
		virtual zenUInt						GetPropertyDefIndex(zenHash32 _hPropertyName)const;
	protected:
		static zenArrayStatic<zenU32>				saIcon;		//!< All mesh share the same icon
	};
	

}

#endif
