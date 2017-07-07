#pragma once

#include <Engine/ThirdParty/imgui/imgui.h>
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
zenClassDeclare(zxRenderData, zRefCounted)
public:
											zxRenderData();
	alignas(16) zenMath::Matrix				matOrthographic;
	zenRes::zGfxTarget2D					mrRendertarget;
	zenRes::zGfxRenderPass					mrRenderpass;
	zenRes::zGfxIndex						mrIndexBuffer;
	zenRes::zGfxStructBuffer<ImDrawVert>	mrVertexBuffer;
	zArrayStatic<zenRes::zShaderResource>	marShaderResources;
	zArrayStatic<zenRes::zGfxCBuffer>		marShaderCBuffers;
	zenSig::zSignalEmitter0					msigRenderUI;
	zVec2U16								mvScreenSize	= zVec2U16(0,0);
	zU32									muVertexCount	= 0; //! @todo Urgent have buffer accessors available 
	zU32									muIndexCount	= 0;
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
zenClassDeclare(zxImGUIHelper, zSingleton<zxImGUIHelper>)
public:
											zxImGUIHelper();
	void									Render(const zEngineRef<zxRenderData>& _rImGuiData, WindowInputState* _pInputData=nullptr);

protected:									
	zenRes::zGfxStateRaster					mrStateRaster;	
	zenRes::zGfxShaderVertex				mrShaderVertex;
	zenRes::zGfxShaderPixel					mrShaderPixel;
	zenRes::zGfxShaderBinding				mrShaderBinding;
	zenRes::zGfxTexture2D					mrFontTextureDefault;
	zenRes::zGfxStateSampler						mrFontSampler;	
	friend class zxRenderData;
};

}
