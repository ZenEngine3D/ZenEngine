#pragma once

#define	ZEN_EXPORT_DX12				(ZEN_RENDERER_DX12)			//Platforms that can export DX11 resources
#define	ZEN_EXPORT_OR_RESOURCE_DX12	(ZEN_EXPORT_DX12)			//Platforms that can use or export DX11 resources

#if ZEN_EXPORT_OR_RESOURCE_DX12
#include <d3d12.h>	
#include <d3d11.h> //@todo 1 remove

namespace zcExp
{
	enum eConstantDX12
	{
		kuDX12_SamplerPerStageMax	= 16,	//!< Maximum supported bound Sampler per shader stage
		kuDX12_CBufferPerStageMax	= 14,	//!< Maximum supported bound Constant Buffer per shader stage
		kuDX12_ResourcesPerStageMax	= 128,	//!< @todo clean up this, remove unused define above
	};
}

#include "Graphic/State/zcExpGfxStateDX12.h"
#include "Graphic/Buffer/zcExpGfxIndexDX12.h"
#include "Graphic/Shader/zcExpGfxShaderDX12.h"
#include "Graphic/Surface/zcExpGfxTextureDX12.h"
#include "Graphic/Surface/zcExpGfxWindowDX12.h"
#endif
