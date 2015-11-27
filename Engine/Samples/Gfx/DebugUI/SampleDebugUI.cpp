#include "zenEngine.h"
#include "SampleDebugUI.h"
//#include <initializer_list>

#include <Engine/ThirdParty/imgui/imgui.h> //! @todo Urgent remove this

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================
namespace sample
{

const zArrayStatic<SimpleVertex> aCubeVertices =
{
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },

	{ zVec3F( -1.0f, -1.0f, -1.0f ),zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },

	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( -1.0f, -1.0f, -1.0f ),zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },

	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },

	{ zVec3F( -1.0f, -1.0f, -1.0f ),zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 0.0f, 0.0f ) },

	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },
	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },
};

const zArrayStatic<zU16> aCubeIndices =
{
	3,1,0,		2,1,3,
	6,4,5,		7,4,6,
	11,9,8,		10,9,11,
	14,12,13,	15,12,14,
	19,17,16,	18,17,19,
	22,20,21,	23,20,22
};

const zArrayStatic<zenRes::zGfxVertex::Element> aCubeVerticeInfos = {
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 3, zenConst::keShaderSemantic_Position,	ZENMemberOffset(SimpleVertex, Pos) ),
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,			ZENMemberOffset(SimpleVertex, Tex) ) 
};

const zArrayStatic<zenRes::zGfxVertex::Element> aUIVerticeInfos = {
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_Position,	ZENMemberOffset(ImDrawVert, pos)),
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,			ZENMemberOffset(ImDrawVert, uv)),
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_UByte, 4, zenConst::keShaderSemantic_Color,		ZENMemberOffset(ImDrawVert, col))
};


bool SampleDebugUIInstance::IsDone()
{
	return false;
}

bool SampleDebugUIInstance::Init()
{
	Super::Init();
	CreateGfxWindow( zVec2U16(1280, 800), zVec2U16(0,0) );
	
	//! @todo Urgent : Finish raster state, per mesh values

	//-----------------------------------------------------------
	// Prepare some data for asset creation
	zArrayStatic<zenRes::zGfxVertex::Stream> aVerticeStreams(1);		
	aVerticeStreams[0].muStride = sizeof(SimpleVertex);
	aVerticeStreams[0].maData.Copy(reinterpret_cast<const zU8*>(aCubeVertices.First()), aCubeVertices.SizeMem());
	aVerticeStreams[0].maElements = aCubeVerticeInfos;

	zArrayStatic<zU8>			aTexRGBA;
	zVec2U16					vTexSize(256,256);
	zenConst::eTextureFormat	eTexFormat = zenConst::keTexFormat_RGBA8;
	aTexRGBA.SetCount( vTexSize.x*vTexSize.y*4 );
	zU8*						pTexCur = aTexRGBA.First();
	for(zUInt line=0; line<vTexSize.y; ++line)
	{
		for(zUInt col=0; col<vTexSize.x; ++col)
		{
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0xFF;
			*pTexCur++ = 1;
		}
	}

	zArrayStatic<zenRes::zShaderDefine> aShaderDefines			= {	zenRes::zShaderDefine("DEFINETEST", "1"), zenRes::zShaderDefine("DEFINETEST1", "0")};
	zArrayStatic<const zenRes::zShaderParameter*> aParamAll		= {	&zenRes::zShaderFloat4(zHash32("vMeshColor"),	zVec4F(.7f,.7f,.7f,1)),
																	&zenRes::zShaderFloat4(zHash32("vColor"),		zVec4F(1,1,1,1)) };
	{
		ImGuiIO& io = ImGui::GetIO();

		// Build
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
		zArrayStatic<zU8> aFontRGBA;
		aFontRGBA.Copy(pixels, width*height * 4);
		mrUITexture = zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, zVec2U16(width, height), aFontRGBA);

		// Store our identifier
		io.Fonts->TexID = 0;

		// Cleanup (don't clear the input data if you want to append new fonts later)
		io.Fonts->ClearInputData();
		io.Fonts->ClearTexData();

		mrUIVS	= zenRes::zGfxShaderVertex::Create("Shader/ImGui.sl", "VSMain");
		mrUIPS	= zenRes::zGfxShaderPixel::Create("Shader/ImGui.sl", "PSMain");
		mrUIShaderBind = zenRes::zGfxShaderBinding::Create(mrUIVS, mrUIPS);

		io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	}


	//---------------------------------------------------------------------
	// Create rendering resources		
	//---------------------------------------------------------------------	
	mrCubeVertex									= zenRes::zGfxVertex::Create(aVerticeStreams, zFlagResUse());	
	mrCubeIndex										= zenRes::zGfxIndex::Create( aCubeIndices, zenConst::kePrimType_TriangleList );
	mrShaderVS										= zenRes::zGfxShaderVertex::Create( "Shader/Tutorial07.fx", "VS");
	mrShaderPS										= zenRes::zGfxShaderPixel::Create( "Shader/Tutorial07.fx", "PS", aShaderDefines );
	mrTexture										= zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	mrSampler										= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Trilinear, zenConst::keTexFilter_Bilinear, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrSampler2										= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);	
	
	// Some bindings of render resource together
	mrShaderBind									= zenRes::zGfxShaderBinding::Create(mrShaderVS, mrShaderPS);
	mrCube3MeshStrip								= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrShaderBind );
	mrStateRaster									= zenRes::zGfxStateRasterizer::Create(zenRes::zGfxStateRasterizer::Config());
		
	//-------------------------------------------------
	// Init some shader values
	//---------------------------------------------------------------------
	zenMath::V4 vEye	= zenMath::simdXYZW( 0.0f, 0.0f,-5.0f, 0.0f );
	zenMath::V4 vAt		= zenMath::simdXYZW( 0.0f, 0.0f, 0.0f, 0.0f );
	zenMath::V4 vUp		= zenMath::simdXYZW( 0.0f, 1.0f, 0.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );		
	matWorld.SetIdentity();
	matWorld.SetPos(zenMath::simdXYZW(3.0f, 0.0f, 0.0f, 1.0f));
	mrCube3MeshStrip.SetValue( aParamAll );
	mrCube3MeshStrip.SetValue( zHash32("View"),		matView );
	mrCube3MeshStrip.SetValue( zHash32("Projection"),matProjection );		
	mrCube3MeshStrip.SetValue( zHash32("World"),	matWorld );
	mrCube3MeshStrip.SetValue( zHash32("txColor"),	mrTexture, mrSampler2);
	mrCube3MeshStrip.SetValue( zHash32("vColor"),	zVec4F(1,1,1,1));

	return true;
}

void SampleDebugUIInstance::UpdateBackbuffers()
{
	if( !mrRndPassFinal.IsValid() || mrMainWindowGfx.PerformResize() )
	{	
		zVec2U16 vBackbufferDim = mrMainWindowGfx.GetBackbuffer().GetDim();
		{	// Rendering to backbuffer
			zenRes::zGfxRenderPass::ConfigColorRT	FinalColorRTConfig;
			zenRes::zGfxRenderPass::ConfigDepthRT	FinalDepthRTConfig;					
			mrBackbufferDepth						= zenRes::zGfxRenderTarget::Create(zenConst::keTexFormat_D24S8, vBackbufferDim ); 
			FinalColorRTConfig.mrTargetSurface		= mrMainWindowGfx.GetBackbuffer();
			FinalDepthRTConfig.mrTargetSurface		= mrBackbufferDepth;
			FinalDepthRTConfig.mbDepthEnable		= true;
			FinalDepthRTConfig.mbDepthWrite			= true;
			FinalDepthRTConfig.meDepthTest			= zenRes::zGfxRenderPass::ConfigDepthRT::keCmpTest_Less;
			mrRndPassFinal							= zenRes::zGfxRenderPass::Create("RenderBackbufferFinal", 2, FinalColorRTConfig, FinalDepthRTConfig, mrStateRaster);	
			zenMath::MatrixProjectionLH( matProjection, 60, float(vBackbufferDim.y)/float(vBackbufferDim.x), 0.01f, 100.f );
		}

		{	// Rendering to UI
			zenRes::zGfxRenderPass::ConfigColorRT	UIColorRTConfig;
			zenRes::zGfxRenderPass::ConfigDepthRT	UIDepthRTConfig;
			zenRes::zGfxStateRasterizer::Config		UIRasterConfig;
			UIRasterConfig.mbScissorEnable			= true;
			UIRasterConfig.mbShowBackface			= true;
			UIRasterConfig.mbShowFrontface			= true;

			mrUIRaster								= zenRes::zGfxStateRasterizer::Create(UIRasterConfig); 
			UIColorRTConfig.mrTargetSurface			= mrMainWindowGfx.GetBackbuffer();
			UIColorRTConfig.mbBlendEnable			= true;
			UIColorRTConfig.meBlendColorSrc			= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_SrcAlpha;
			UIColorRTConfig.meBlendColorDest		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_InvSrcAlpha;
			UIColorRTConfig.meBlendColorOp			= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;
			UIColorRTConfig.meBlendAlphaSrc			= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_Zero;
			UIColorRTConfig.meBlendAlphaDest		= zenRes::zGfxRenderPass::ConfigColorRT::keBlendVal_One;
			UIColorRTConfig.meBlendAlphaOp			= zenRes::zGfxRenderPass::ConfigColorRT::keBlendOp_Add;
			mrUIRndPass								= zenRes::zGfxRenderPass::Create("RenderUI", 100, UIColorRTConfig, UIDepthRTConfig, mrUIRaster);
			zenMath::MatrixProjectionOrthoLH(matUIOrthographic, vBackbufferDim.x, vBackbufferDim.y, 0, 1);
		}
	}
}

void SampleDebugUIInstance::Destroy()
{
	Super::Destroy();
}

void SampleDebugUIInstance::Update()
{	
	Super::Update();
	{

	
	zArrayDynamic<zenRes::zGfxDrawcall> aDrawcalls;
	aDrawcalls.Reserve(1000);
	UpdateBackbuffers();

#if 1
	//============================================================================	
	// Start the frame
	ImGui::NewFrame();
	ImGui::ShowTestWindow();
	TempUpdateUIFps();
	ImGui::Render();
	ImDrawData* pUiData = ImGui::GetDrawData();
	static zUInt VtxCount = 0; //! @todo Urgent have buffer accessors available 
	static zUInt IdxCount = 0;

	// UI: Grow Vertex/Index buffer when needed
	if (VtxCount < pUiData->TotalVtxCount)
	{
		zArrayStatic<zenRes::zGfxVertex::Stream> aUIVerticeStreams(1);
		VtxCount						= static_cast<zUInt>(pUiData->TotalVtxCount*1.25);		
		aUIVerticeStreams[0].muStride	= static_cast<zU32>(sizeof(ImDrawVert));		
		aUIVerticeStreams[0].maElements	= aUIVerticeInfos;
		aUIVerticeStreams[0].maData.SetCount(VtxCount*sizeof(ImDrawVert));
		mrUIVertex						= zenRes::zGfxVertex::Create(aUIVerticeStreams, zFlagResUse(zenConst::keResUse_DynamicDiscard));
	}
	if (IdxCount < pUiData->TotalIdxCount)
	{		
		IdxCount = static_cast<zUInt>(pUiData->TotalIdxCount*1.25);
		if( sizeof(ImDrawIdx) == sizeof(zU16) )
		{
			zArrayStatic<zU16> aIndices;
			aIndices.SetCount(IdxCount);
			mrUIIndex = zenRes::zGfxIndex::Create(aIndices, zenConst::kePrimType_TriangleList);
		}
		else if (sizeof(ImDrawIdx) == sizeof(zU32))
		{
			zArrayStatic<zU32> aIndices;
			aIndices.SetCount(IdxCount);
			mrUIIndex = zenRes::zGfxIndex::Create(aIndices, zenConst::kePrimType_TriangleList);
		}		
	}
	
	// UI: Update content of vertex/index
	ImDrawVert* pUIVertices	= reinterpret_cast<ImDrawVert*>(mrUIVertex.Lock());
	ImDrawIdx* pUIIndices	= reinterpret_cast<ImDrawIdx*>(mrUIIndex.Lock());
	for (int n = 0; n < pUiData->CmdListsCount; ++n)
	{
		const ImDrawList* cmd_list = pUiData->CmdLists[n];
		zenMem::Copy(pUIVertices, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size());
		zenMem::Copy(pUIIndices, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size());
		pUIVertices += cmd_list->VtxBuffer.size();
		pUIIndices += cmd_list->IdxBuffer.size();
	}
	mrUIVertex.Unlock();
	mrUIIndex.Unlock();
	
	// UI: Emit drawcalls
	int vtx_offset = 0;
	int idx_offset = 0;
	zArrayDynamic<zenRes::zGfxMeshStrip> aUIMeshStrip;
	aUIMeshStrip.Reserve(pUiData->CmdListsCount*2);
	for (int n = 0; n < pUiData->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = pUiData->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				zenRes::zGfxMeshStrip rMeshStrip = zenRes::zGfxMeshStrip::Create(mrUIVertex, mrUIIndex, mrUIShaderBind, idx_offset, pcmd->ElemCount, vtx_offset);
				//! @todo optim create shaderparam and reuse it instead of recreating it for all mesh strip
				rMeshStrip.SetValue(zHash32("txFont"), mrUITexture, mrSampler);
				rMeshStrip.SetValue(zHash32("ProjectionMatrix"), matUIOrthographic);
				rMeshStrip.Draw(mrUIRndPass, float(aDrawcalls.Count()), aDrawcalls, zVec4U16( zU16(pcmd->ClipRect.x), zU16(pcmd->ClipRect.y), zU16(pcmd->ClipRect.z), zU16(pcmd->ClipRect.w)));
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.size();
	}
	//mrUIMesh = zenRes::zGfxMesh::Create(aUIMeshStrip);
	//mrUIMesh.SetValue(zHash32("txFont"), mrUITexture, mrSampler );
	//mrUIMesh.SetValue(zHash32("ProjectionMatrix"), matUIOrthographic);
	//============================================================================
#endif
	// Testing memory leaking
	#if 0
	{
		zenRes::zGfxMeshStrip rMeshStrip = zenRes::zGfxMeshStrip::Create(mrCubeVertex, mrCubeIndex, mrShaderBind );
		static int i(0);
		i++;
	}
	#endif
	
	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------	
	mrMainWindowGfx.FrameBegin();
	
	float t = static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation

	zVec4F vClearColor = zenMath::TriLerp( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
	aDrawcalls.Push( zenRes::zGfxDrawcall::ClearColor(mrRndPassFinal, 0, mrMainWindowGfx.GetBackbuffer(), vClearColor) );
	aDrawcalls.Push( zenRes::zGfxDrawcall::ClearDepthStencil(mrRndPassFinal, 0, mrBackbufferDepth) );

	matWorld.SetRotationY( t );	// Rotate cube around the origin 				
	mrCube3MeshStrip.SetValue( zHash32("World"), matWorld );
	mrCube3MeshStrip.SetValue( zHash32("Projection"), matProjection );
	mrCube3MeshStrip.Draw(mrRndPassFinal, 0, aDrawcalls);

	zenRes::zGfxDrawcall::Submit(aDrawcalls);
	mrMainWindowGfx.FrameEnd();
	}


}
}
