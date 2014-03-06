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
		virtual PropertyArray&					GetPropertyDef() const;
		virtual zenUInt							GetPropertyDefIndex(zenHash32 _hPropertyName)const;
		virtual const zenAss::PropertyArray&	GetProperties()const{ static zenAss::PropertyArray test(0); return test;};
	protected:
		static zenArrayStatic<zenU32>	saIcon;		//!< All mesh share the same icon
	};
}} //namespace zen { namespace zeAss

#endif
#endif
