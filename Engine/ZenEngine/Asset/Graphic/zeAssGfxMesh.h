#pragma once
#ifndef __LibZenEngine_Asset_Gfx_Mesh_h__
#define __LibZenEngine_Asset_Gfx_Mesh_h__

#if ZEN_ENGINETOOL

namespace zen { namespace zeAss
{
	class GfxMesh : public AssetTyped<zenConst::keAssType_Mesh>
	{
	ZENClassDeclare(GfxMesh, Asset)
	public:
		virtual const zenAss::PropertyDefArray&	GetProperties()const;
	};
}} //namespace zen { namespace zeAss

#endif
#endif
