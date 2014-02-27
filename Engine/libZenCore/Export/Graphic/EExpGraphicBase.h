#pragma once
#ifndef __LibExport_Gfx_Base_h__
#define __LibExport_Gfx_Base_h__ 

namespace EExp
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

#include ZENHeaderPlatform(EExpGraphicBase)

#include <Engine/libZenCore/Export/Graphic/Surface/EExpTexture.h>
#include <Engine/libZenCore/Export/Graphic/Surface/EExpState.h>
#include <Engine/libZenCore/Export/Graphic/Surface/EExpRenderTarget.h>
#include <Engine/libZenCore/Export/Graphic/Surface/EExpView.h>
#include <Engine/libZenCore/Export/Graphic/Surface/EExpWindow.h>
#include <Engine/libZenCore/Export/Graphic/Mesh/EExpVertex.h>
#include <Engine/libZenCore/Export/Graphic/Mesh/EExpIndex.h>
#include <Engine/libZenCore/Export/Graphic/Mesh/EExpMeshStrip.h>
#include <Engine/libZenCore/Export/Graphic/Mesh/EExpMesh.h>
#include <Engine/libZenCore/Export/Graphic/Shader/EExpShader.h>
#include <Engine/libZenCore/Export/Graphic/Shader/EExpShaderParam.h>
#include <Engine/libZenCore/Export/Graphic/Shader/EExpShaderParamDef.h>
#include <Engine/libZenCore/Export/Graphic/Binding/EExpShaderInputSignature.h>
#include <Engine/libZenCore/Export/Graphic/Binding/EExpInputStreamBinding.h>
#include <Engine/libZenCore/Export/Graphic/Binding/EExpShaderBinding.h>

#endif
