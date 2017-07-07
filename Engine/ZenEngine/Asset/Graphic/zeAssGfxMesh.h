#pragma once

#if ZEN_ENGINETOOL

namespace zeAss
{
	class GfxMesh : public AssetTyped<zenConst::keAssType_Mesh>
	{
	zenClassDeclare(GfxMesh, Asset)
	public:
		virtual const zenAss::PropertyDefArray&	GetProperties()const;
	};
} //namespace zeAss

#endif
