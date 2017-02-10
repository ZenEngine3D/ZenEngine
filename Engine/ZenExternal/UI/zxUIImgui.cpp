#include "zxExternal.h"

namespace zxImGui
{

zxImGUIHelper::zxImGUIHelper()
{
	// Shaders & State resources
	zenRes::zGfxStateRaster::Config	RasterConfig;
	RasterConfig.mbScissorEnable	= true;
	RasterConfig.mbShowBackface		= true;
	RasterConfig.mbShowFrontface	= true;
	mrShaderVertex					= zenRes::zGfxShaderVertex::Create("Shader/ImGui.sl", "VSMain");
	mrShaderPixel					= zenRes::zGfxShaderPixel::Create("Shader/ImGui.sl", "PSMain");
	mrShaderBinding					= zenRes::zGfxShaderBinding::Create(mrShaderVertex, mrShaderPixel);
	mrFontSampler					= zenRes::zGfxStateSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrStateRaster					= zenRes::zGfxStateRaster::Create(RasterConfig);
	
	// Font Texture
	unsigned char* pixels;
	int width, height;
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	zArrayStatic<zU8> aFontRGBA;
	aFontRGBA.Copy(pixels, width*height * 4);
#if !ZEN_RENDERER_DX12
	mrFontTextureDefault			= zenRes::zGfxTexture2D::Create(zenConst::keTexFormat_RGBA8, zVec2U16(width, height), aFontRGBA);
#endif
	// imGUI config	
	io.Fonts->TexID					= 0;	// Store our identifier (Always same texture at the moment)
	io.Fonts->ClearInputData();				// Cleanup (don't clear the input data if you want to append new fonts later)
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
}

//=================================================================================================
//! @details : Prep Shader resources array to initialize newly created mesh strip instead of
//				having to create new ConstantBuffers every time and manually assign resources
//-------------------------------------------------------------------------------------------------
//=================================================================================================
zxRenderData::zxRenderData()
{
	// Create resources array, for assigning to created mesh strip
	zxImGUIHelper::Get().mrShaderBinding.CreateShaderParam(marShaderCBuffers);
	marShaderResources.SetCount(3 + marShaderCBuffers.Count() );	
	marShaderResources[0].mhResourceName		= zHash32("VInputAll");
	marShaderResources[0].mResourceID			= zResID(); //Will be assigned on VertexCreation in zxImGuiHelper::Render
	marShaderResources[1].mhResourceName		= zHash32("txFont.mTexture");
	marShaderResources[1].mResourceID			= zxImGUIHelper::Get().mrFontTextureDefault.GetResID();
	marShaderResources[2].mhResourceName		= zHash32("txFont.mSampler");
	marShaderResources[2].mResourceID			= zxImGUIHelper::Get().mrFontSampler.GetResID();
	for(zUInt idx(0), count(marShaderCBuffers.Count()); idx<count; ++idx)
		marShaderResources[3+idx].mResourceID	= marShaderCBuffers[idx].GetResID();
}

//=================================================================================================
//-------------------------------------------------------------------------------------------------
//=================================================================================================
void zxImGUIHelper::Render(const zEngineRef<zxRenderData>& _rImGuiData, WindowInputState* _pInputData)
{	
	zenPerf::zScopedEventCpu EmitEvent("ImGUI");

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

		for (zUInt idx(0), count(zenMath::Min(_pInputData->mbIsKeyDown.size(), zenArrayCount(io.KeysDown))); idx<count; ++idx)
			io.KeysDown[idx] = _pInputData->mbIsKeyDown[idx];

		for (zUInt idx(0), count(_pInputData->maCharacterPressed.Count()); idx<count; ++idx)
			if (_pInputData->maCharacterPressed[idx] > 0 && _pInputData->maCharacterPressed[idx] < 0x10000)
				io.AddInputCharacter(_pInputData->maCharacterPressed[idx]);
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

		// Not sure which CBuffer has this parameter, assign it to all
		for(zUInt idx(0), count(_rImGuiData->marShaderCBuffers.Count()); idx<count; ++idx)
			_rImGuiData->marShaderCBuffers[idx].SetValue(zHash32("ProjectionMatrix"), _rImGuiData->matOrthographic);
	}

	
	//----------------------------------------------------------------------------
	// Ask all client subscriber to display their UI
	io.DisplaySize				= ImVec2(_rImGuiData->mvScreenSize.x, _rImGuiData->mvScreenSize.y);	
	ImGui::NewFrame();	
	{
		zenPerf::zScopedEventCpu EmitEvent("Render callbacks");
		_rImGuiData->msigRenderUI.Emit();
	}
	{
		zenPerf::zScopedEventCpu EmitEvent("Internal Render");
		ImGui::Render();		
	}

	ImDrawData* pImGuiData		= ImGui::GetDrawData();
	zenGfx::zContext rUIContext	= zenGfx::zContext::Create("ImGui", _rImGuiData->mrRenderpass);
	//----------------------------------------------------------------------------
	// Generates Vertex and Index buffer	
	{	
		zenPerf::zScopedEventCpu EmitEvent("Update Vertex/Index");
		//----------------------------------------------------------------------------
		// Grow Vertex/Index buffer when needed		
		if(_rImGuiData->muVertexCount < static_cast<zU32>(pImGuiData->TotalVtxCount) )
		{
			_rImGuiData->muVertexCount						= static_cast<zU32>(pImGuiData->TotalVtxCount*1.25f);
			_rImGuiData->mrVertexBuffer						= zenRes::zGfxStructBuffer<ImDrawVert>::Create(_rImGuiData->muVertexCount);			
			_rImGuiData->marShaderResources[0].mResourceID	= _rImGuiData->mrVertexBuffer.GetResID();
		}

		if(_rImGuiData->muIndexCount < static_cast<zU32>(pImGuiData->TotalIdxCount))
		{
			_rImGuiData->muIndexCount = static_cast<zU32>(pImGuiData->TotalIdxCount*1.25f);
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
		if( _rImGuiData->muIndexCount && _rImGuiData->muVertexCount )
		{		
			ImDrawVert* pUIVertices = _rImGuiData->mrVertexBuffer.Lock();
			ImDrawIdx* pUIIndices	= reinterpret_cast<ImDrawIdx*>(_rImGuiData->mrIndexBuffer.Lock());
			for (int n = 0; n < pImGuiData->CmdListsCount; ++n)
			{
				const ImDrawList* cmd_list = pImGuiData->CmdLists[n];
				zenMem::Copy(pUIVertices, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size());
				zenMem::Copy(pUIIndices, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size());
				pUIVertices += cmd_list->VtxBuffer.size();
				pUIIndices += cmd_list->IdxBuffer.size();
			}
			_rImGuiData->mrVertexBuffer.Unlock(rUIContext);
			_rImGuiData->mrIndexBuffer.Unlock(rUIContext);
		}
	}

	//----------------------------------------------------------------------------
	// Emit drawcalls
	{		
		zenPerf::zScopedEventCpu EmitEvent("Emit Drawcalls");		
		int vtx_offset				= 0;
		int idx_offset				= 0;
		bool bFirst					= true;
		for (int n = 0; n < pImGuiData->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list			= pImGuiData->CmdLists[n];
			zenRes::zGfxMeshStrip rMeshStrip	= zenRes::zGfxMeshStrip::Create(_rImGuiData->mrIndexBuffer, mrShaderBinding, _rImGuiData->marShaderResources, 0, pImGuiData->TotalIdxCount, vtx_offset);
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
			{
				zenPerf::zScopedEventCpu EmitEvent("ImGUI Draw test");
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					zenGfx::zCommand::DrawMesh(rUIContext, 0.f, rMeshStrip, idx_offset, pcmd->ElemCount, zVec4U16(zU16(pcmd->ClipRect.x), zU16(pcmd->ClipRect.y), zU16(pcmd->ClipRect.z), zU16(pcmd->ClipRect.w)));
				}

				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.size();
		}

		rUIContext.Submit();
	}
}

} // namespace zxImGui