#pragma once
#ifndef __LibZenExternal_IMGUI_h__
#define __LibZenExternal_IMGUI_h__

struct WindowInputState; //Forward declare

namespace zxImGui
{
//=================================================================================================
//! @class	zxRenderData 
//-------------------------------------------------------------------------------------------------
//! @brief	Used to store persistent IMGui data needed for rendering
//! @note	mrRendertarget is the only value needing configured value when calling Render, 
//! 		remaining values are auto managed
//=================================================================================================
class zxRenderData : public zRefCounted
{
ZENClassDeclare(zxRenderData, zRefCounted)
public:
	alignas(16) zenMath::Matrix				matOrthographic;
	zenRes::zGfxTarget2D					mrRendertarget;
	zenRes::zGfxRenderPass					mrRenderpass;
	zenRes::zGfxVertex						mrVertexBuffer;
	zenRes::zGfxIndex						mrIndexBuffer;
	zenSig::zSignalEmitter0					msigRenderUI;	
	zVec2U16								mvScreenSize	= zVec2U16(0,0);
	zUInt									muVertexCount	= 0; //! @todo Urgent have buffer accessors available 
	zUInt									muIndexCount	= 0;
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
class zxImGUIHelper : public zSingleton<zxImGUIHelper>
{
ZENClassDeclare(zxImGUIHelper, zSingleton<zxImGUIHelper>)
public:
											zxImGUIHelper();
	void									Render(const zEngineRef<zxRenderData>& _rImGuiData, WindowInputState* _pInputData=nullptr);

protected:									
	zenRes::zGfxStateRaster					mrStateRaster;	
	zenRes::zGfxShaderVertex				mrShaderVertex;
	zenRes::zGfxShaderPixel					mrShaderPixel;
	zenRes::zGfxShaderBinding				mrShaderBinding;
	zArrayStatic<zenRes::zGfxShaderParam>	marShaderParams;
	zenRes::zGfxTexture2d					mrFontTextureDefault;
	zenRes::zGfxSampler						mrFontSampler;
};

}

#endif