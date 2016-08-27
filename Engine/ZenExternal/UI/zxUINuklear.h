#pragma once
#ifndef __LibZenExternal_UI_Nuklear_h__
#define __LibZenExternal_UI_Nuklear_h__

struct WindowInputState; //Forward declare

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <Engine/ThirdParty/nuklear/nuklear.h>

namespace zxNuklear
{
//=================================================================================================
//! @class	zxRenderData 
//-------------------------------------------------------------------------------------------------
//! @brief	Used to store persistent IMGui data needed for rendering
//! @note	mrRendertarget is the only value needing configuration value when calling Render, 
//! 		remaining values are auto managed
//=================================================================================================
class zxRenderData : public zRefCounted
{
zenClassDeclare(zxRenderData, zRefCounted)
public:
											zxRenderData();
											~zxRenderData();
	alignas(16) zenMath::Matrix				matOrthographic;
	zenRes::zGfxTarget2D					mrRendertarget		= nullptr;	
	zenRes::zGfxRenderPass					mrRenderpass		= nullptr;		
	zenRes::zGfxVertex						mrVertexBuffer		= nullptr;
	zenRes::zGfxIndex						mrIndexBuffer		= nullptr;	
	zenRes::zGfxTexture2d					mrFontTextureAtlas	= nullptr;
	zenSig::zSignalEmitter0					msigRenderUI;	
	zVec2U16								mvScreenSize		= zVec2U16(0,0);
	zUInt									muVertexCount		= 0; //! @todo Urgent have buffer accessors available 
	zUInt									muIndexCount		= 0;
	
	nk_convert_config						moConfig;
	nk_context								moContext;
    nk_font_atlas							moAtlas;
    nk_buffer								moCommands;
	nk_font_atlas							moFontAtlas;
	nk_draw_null_texture					moDrawNullTexture;	
};

//=================================================================================================
//! @class		zxImGUIHelper
//-------------------------------------------------------------------------------------------------
//! @brief		Use this wrapper over the imGUI library, to render UI.
//!	@details	The imGUI library is a in-engine UI that allow nice basic UI capabilities
//!				that is useful for debugging. This class Encapsulate all shared resources, 
//!				between all UI rendering.
//! @note		https://github.com/ocornut/imgui
//=================================================================================================
class zxNuklearHelper : public zSingleton<zxNuklearHelper>
{
zenClassDeclare(zxNuklearHelper, zSingleton<zxNuklearHelper>)
public:
											zxNuklearHelper();
	void									Render(const zEngineRef<zxRenderData>& _rNuklearData, WindowInputState* _pInputData=nullptr);

protected:									
	zenRes::zGfxStateRaster				mrStateRaster;	
	zenRes::zGfxShaderVertex				mrShaderVertex;
	zenRes::zGfxShaderPixel					mrShaderPixel;
	zenRes::zGfxShaderBinding				mrShaderBinding;
	zArrayStatic<zenRes::zGfxShaderParam>	marShaderParams;	
	zenRes::zGfxSampler						mrFontSampler;	
};

}

#endif //__LibZenExternal_UI_Nuklear_h__