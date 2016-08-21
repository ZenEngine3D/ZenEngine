#include "zcCore.h"

namespace zcRes
{
	const zArrayStatic<GfxMeshStripRef>& GfxMesh::GetMeshStrips() const
	{
		return marGfxMeshStrip;
	}

}
