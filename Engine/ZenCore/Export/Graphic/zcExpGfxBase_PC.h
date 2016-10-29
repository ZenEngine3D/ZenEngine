#if ZEN_EXPORT_OR_RESOURCE_DX11
#include <d3d11.h>	

namespace zcExp
{
	enum eConstantDX11
	{
	//	kuDX11_TexturePerStageMax	= 64,	//!< Maximum supported bound Texture per shader stage
		kuDX11_SamplerPerStageMax	= 16,	//!< Maximum supported bound Sampler per shader stage
//		kuDX11_BufferPerStageMax	= 64,	//!< Maximum supported bound StructBuffer/UAV per shader stage (@todo clean real limit is 128 resources of any type, update this)
		kuDX11_CBufferPerStageMax	= 14,	//!< Maximum supported bound Constant Buffer per shader stage
		kuDX11_ResourcesPerStageMax	= 128,	//!< @todo clean up this, remove unused define above
	};
}

#endif
