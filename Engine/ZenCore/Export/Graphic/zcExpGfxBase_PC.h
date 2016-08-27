
#if ZEN_EXPORT_OR_RESOURCE_DX11
	#include <d3d11.h>	

namespace zcExp
{
	enum eConstantDX11
	{
		kuDX11_TexturePerStageMax	= 16,	//!< Maxinum supported texture per shader stage
		kuDX11_BufferPerStageMax	= 16,	//! @todo clean real limit is 128 resources of any type, update this
	};
}

#endif



