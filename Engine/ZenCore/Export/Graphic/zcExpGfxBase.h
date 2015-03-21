#pragma once
#ifndef __zCore_Exp_Gfx_Base_h__
#define __zCore_Exp_Gfx_Base_h__ 

namespace zcExp
{
	enum eShaderStage
	{
		keShaderStage_Vertex, 
		keShaderStage_Pixel,
		keShaderStage__Count,
		keShaderStage__Invalid
	};

	enum eShaderParamFreq
	{
		keShaderParamFreq_Global		= 0,
		keShaderParamFreq_PerViewPhase	= 1,		
		keShaderParamFreq_PerMeshOnce	= 2,
		keShaderParamFreq_PerMesh		= 3,
		keShaderParamFreq_PerStripOnce	= 4,
		keShaderParamFreq_PerStrip		= 5,
		keShaderParamFreq__Count,
		keShaderParamFreq__Invalid
	};


}

#include ZENHeaderPlatform(zcExpGfxBase)

#include "Graphic/Surface/zcExpGfxTexture.h"
#include "Graphic/Surface/zcExpGfxState.h"
#include "Graphic/Surface/zcExpGfxRenderTarget.h"
#include "Graphic/Surface/zcExpGfxView.h"
#include "Graphic/Surface/zcExpGfxWindow.h"
#include "Graphic/Mesh/zcExpGfxVertex.h"
#include "Graphic/Mesh/zcExpGfxIndex.h"
#include "Graphic/Mesh/zcExpGfxMeshStrip.h"
#include "Graphic/Mesh/zcExpGfxMesh.h"
#include "Graphic/Shader/zcExpGfxShader.h"
#include "Graphic/Shader/zcExpGfxShaderParam.h"
#include "Graphic/Shader/zcExpGfxShaderParamDef.h"
#include "Graphic/Binding/zcExpGfxShaderInputSignature.h"
#include "Graphic/Binding/zcExpGfxInputStreamBinding.h"
#include "Graphic/Binding/zcExpGfxShaderBinding.h"

#endif
