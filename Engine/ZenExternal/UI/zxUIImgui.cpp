#include "zxExternal.h"
#include <Engine/ThirdParty/imgui/imgui.h>

namespace zxImGui
{

const zArrayStatic<zenRes::zGfxVertex::Element> aUIVerticeInfos = {
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_Position,	ZENMemberOffset(ImDrawVert, pos)),
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,			ZENMemberOffset(ImDrawVert, uv)),
		zenRes::zGfxVertex::Element(zenConst::keShaderElemType_UByte, 4, zenConst::keShaderSemantic_Color,		ZENMemberOffset(ImDrawVert, col))
	};

zxImGUIHelper::zxImGUIHelper()
{
	// Shaders & State resources
	zenRes::zGfxStateRasterizer::Config	RasterConfig;
	RasterConfig.mbScissorEnable	= true;
	RasterConfig.mbShowBackface		= true;
	RasterConfig.mbShowFrontface	= true;
	mrShaderVertex					= zenRes::zGfxShaderVertex::Create("Shader/ImGui.sl", "VSMain");
	mrShaderPixel					= zenRes::zGfxShaderPixel::Create("Shader/ImGui.sl", "PSMain");
	mrShaderBinding					= zenRes::zGfxShaderBinding::Create(mrShaderVertex, mrShaderPixel);
	mrFontSampler					= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrStateRaster					= zenRes::zGfxStateRasterizer::Create(RasterConfig);
	
	// Font Texture
	unsigned char* pixels;
	int width, height;
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	zArrayStatic<zU8> aFontRGBA;
	aFontRGBA.Copy(pixels, width*height * 4);
	mrFontTextureDefault = zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, zVec2U16(width, height), aFontRGBA);

	// imGUI config	
	io.Fonts->TexID					= 0; // Store our identifier

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();

	//! @todo clean create platform abstraction for input
	io.KeyMap[ImGuiKey_Tab]			= VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow]	= VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow]	= VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow]		= VK_UP;
	io.KeyMap[ImGuiKey_DownArrow]	= VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp]		= VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown]	= VK_NEXT;
	io.KeyMap[ImGuiKey_Home]		= VK_HOME;
	io.KeyMap[ImGuiKey_End]			= VK_END;
	io.KeyMap[ImGuiKey_Delete]		= VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace]	= VK_BACK;
	io.KeyMap[ImGuiKey_Enter]		= VK_RETURN;
	io.KeyMap[ImGuiKey_Escape]		= VK_ESCAPE;

	ImGuiStyle& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.0f, 0.00f, 0.10f);
}

//=================================================================================================
//-------------------------------------------------------------------------------------------------
//=================================================================================================
void zxImGUIHelper::Render(const zEngineRef<zxRenderData>& _rImGuiData, WindowInputState* _pInputData)
{	
	//----------------------------------------------------------------------------
	// Update UI input if provided
	ImGuiIO& io = ImGui::GetIO();
	if(_pInputData )
	{
		io.MousePos.x	= (zI16)_pInputData->mvMousePos.x;
		io.MousePos.y	= (zI16)_pInputData->mvMousePos.y;
		io.MouseWheel	= _pInputData->mfMouseWheel;
		io.KeyCtrl		= _pInputData->mbIsKeyDown[VK_CONTROL];
		io.KeyShift		= _pInputData->mbIsKeyDown[VK_SHIFT];
		io.KeyAlt		= _pInputData->mbIsKeyDown[VK_MENU];

		for (zUInt idx(0); idx<WindowInputState::keMouseBtn__Count; ++idx)
			io.MouseDown[idx] = _pInputData->mbIsMouseDown[idx];

		for (zUInt idx(0), count(zenMath::Min(_pInputData->mbIsKeyDown.size(), ZENArrayCount(io.KeysDown))); idx<count; ++idx)
			io.KeysDown[idx] = _pInputData->mbIsKeyDown[idx];

		for (zUInt idx(0), count(_pInputData->maCharacterPressed.Count()); idx<count; ++idx)
			if (_pInputData->maCharacterPressed[idx] > 0 && _pInputData->maCharacterPressed[idx] < 0x10000)
				io.AddInputCharacter(_pInputData->maCharacterPressed[idx]);
	}
	else
	{
		io.MousePos.x	= 0;
		io.MousePos.y	= 0;
		io.MouseWheel	= 0.f;
		io.KeyCtrl		= false;
		io.KeyShift		= false;
		io.KeyAlt		= false;

		for (zUInt idx(0); idx<WindowInputState::keMouseBtn__Count; ++idx)
			io.MouseDown[idx] = false;

		for (zUInt idx(0), count(ZENArrayCount(io.KeysDown)); idx<count; ++idx)
			io.KeysDown[idx] = false;
	}
		
	//----------------------------------------------------------------------------
	// Adjust the rendering size to window size
	if( !_rImGuiData->mrRenderpass.IsValid() || _rImGuiData->mvScreenSize != _rImGuiData->mrRendertarget.GetDim())
	{
		zenRes::zGfxRenderPass::ConfigColorRT	UIColorRTConfig;
		zenRes::zGfxRenderPass::ConfigDepthRT	UIDepthRTConfig;
		_rImGuiData->mvScreenSize			= _rImGuiData->mrRendertarget.GetDim();
		UIColorRTConfig.mrTargetSurface		= _rImGuiData->mrRendertarget;
		UIColorRTConfig.mbBlendEnable		= true;
		UIColorRTConfig.meBlendColorSrc		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_SrcAlpha;
		UIColorRTConfig.meBlendColorDest	= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_InvSrcAlpha;
		UIColorRTConfig.meBlendColorOp		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;
		UIColorRTConfig.meBlendAlphaSrc		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_Zero;
		UIColorRTConfig.meBlendAlphaDest	= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_One;
		UIColorRTConfig.meBlendAlphaOp		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;		
		_rImGuiData->mrRenderpass			= zenRes::zGfxRenderPass::Create("RenderUI", 0, UIColorRTConfig, UIDepthRTConfig, mrStateRaster); //! @todo feature expose name
		zenMath::MatrixProjectionOrthoLH(_rImGuiData->matOrthographic, _rImGuiData->mvScreenSize.x, _rImGuiData->mvScreenSize.y, 0, 1);
	}

	//----------------------------------------------------------------------------
	// Ask all client suscriber to display their UI
	io.DisplaySize = ImVec2(_rImGuiData->mvScreenSize.x, _rImGuiData->mvScreenSize.y);
	ImGui::NewFrame();	
	_rImGuiData->msigRenderUI.Emit();
	ImGui::Render();
	ImDrawData* pImGuiData = ImGui::GetDrawData();

	//----------------------------------------------------------------------------
	// Grow Vertex/Index buffer when needed
	if(_rImGuiData->muVertexCount < pImGuiData->TotalVtxCount)
	{
		zArrayStatic<zenRes::zGfxVertex::Stream> aUIVerticeStreams(1);
		_rImGuiData->muVertexCount = static_cast<zUInt>(pImGuiData->TotalVtxCount*1.25);
		aUIVerticeStreams[0].muStride = static_cast<zU32>(sizeof(ImDrawVert));
		aUIVerticeStreams[0].maElements = aUIVerticeInfos;
		aUIVerticeStreams[0].maData.SetCount(_rImGuiData->muVertexCount*sizeof(ImDrawVert));
		_rImGuiData->mrVertexBuffer = zenRes::zGfxVertex::Create(aUIVerticeStreams, zFlagResUse(zenConst::keResUse_DynamicDiscard));
	}
	if(_rImGuiData->muIndexCount < pImGuiData->TotalIdxCount)
	{
		_rImGuiData->muIndexCount = static_cast<zUInt>(pImGuiData->TotalIdxCount*1.25);
		if (sizeof(ImDrawIdx) == sizeof(zU16))
		{
			zArrayStatic<zU16> aIndices;
			aIndices.SetCount(_rImGuiData->muIndexCount);
			_rImGuiData->mrIndexBuffer = zenRes::zGfxIndex::Create(aIndices, zenConst::kePrimType_TriangleList);
		}
		else if (sizeof(ImDrawIdx) == sizeof(zU32))
		{
			zArrayStatic<zU32> aIndices;
			aIndices.SetCount(_rImGuiData->muIndexCount);
			_rImGuiData->mrIndexBuffer = zenRes::zGfxIndex::Create(aIndices, zenConst::kePrimType_TriangleList);
		}
	}

	//----------------------------------------------------------------------------
	// Update content of vertex/index
	ImDrawVert* pUIVertices = reinterpret_cast<ImDrawVert*>(_rImGuiData->mrVertexBuffer.Lock());
	ImDrawIdx* pUIIndices = reinterpret_cast<ImDrawIdx*>(_rImGuiData->mrIndexBuffer.Lock());
	for (int n = 0; n < pImGuiData->CmdListsCount; ++n)
	{
		const ImDrawList* cmd_list = pImGuiData->CmdLists[n];
		zenMem::Copy(pUIVertices, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size());
		zenMem::Copy(pUIIndices, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size());
		pUIVertices += cmd_list->VtxBuffer.size();
		pUIIndices += cmd_list->IdxBuffer.size();
	}
	_rImGuiData->mrVertexBuffer.Unlock();
	_rImGuiData->mrIndexBuffer.Unlock();

	//----------------------------------------------------------------------------
	// Emit drawcalls
	int vtx_offset = 0;
	int idx_offset = 0;
	zArrayDynamic<zenRes::zGfxDrawcall> aDrawcalls;
	aDrawcalls.Reserve(pImGuiData->CmdListsCount);
	for (int n = 0; n < pImGuiData->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = pImGuiData->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				zenRes::zGfxMeshStrip rMeshStrip = zenRes::zGfxMeshStrip::Create(_rImGuiData->mrVertexBuffer, _rImGuiData->mrIndexBuffer, mrShaderBinding, idx_offset, pcmd->ElemCount, vtx_offset);
				//! @todo optim create shaderparam and reuse it instead of recreating it for all mesh strip
				rMeshStrip.SetValue(zHash32("txFont"), mrFontTextureDefault, mrFontSampler);
				rMeshStrip.SetValue(zHash32("ProjectionMatrix"), _rImGuiData->matOrthographic);
				rMeshStrip.Draw(_rImGuiData->mrRenderpass, float(aDrawcalls.Count()), aDrawcalls, zVec4U16(zU16(pcmd->ClipRect.x), zU16(pcmd->ClipRect.y), zU16(pcmd->ClipRect.z), zU16(pcmd->ClipRect.w)));
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.size();
	}

	zenRes::zGfxDrawcall::Submit(aDrawcalls);
}

} // namespace zxImGui