#include "zcCore.h"

namespace zcRes
{
	GfxShaderBinding::GfxShaderBinding()
	{
		static zU32 suCreatedShaderBinding(0);
		muRenderPassID = ++suCreatedShaderBinding;
	}

	const zArrayStatic<GfxMeshStripRef>& GfxMesh::GetMeshStrips()const
	{
		return marGfxMeshStrip;
	}

}
