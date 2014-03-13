#pragma once
#ifndef __LibZenEngine_Asset_Gfx_Mesh_h__
#define __LibZenEngine_Asset_Gfx_Mesh_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
	class GfxMesh : public AssetItem
	{
	ZENClassDeclare(GfxMesh, AssetItem)
	public:
		virtual zenConst::eAssetType			GetType()const{return zenConst::keAssType_Mesh;}
		virtual const zenAss::PropertyArray&	GetProperties()const;
		virtual zInt							GetValueIndex(zHash32 _hPropertyName)const;
	protected:
		static zArrayStatic<zU32>	saIcon;		//!< All mesh share the same icon @todo Keep this?
	};
}} //namespace zen { namespace zeAss

#endif
#endif
