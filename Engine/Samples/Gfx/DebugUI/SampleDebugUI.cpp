#include "zenEngine.h"
#include "SampleDebugUI.h"

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
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 3, zenConst::keShaderSemantic_Position,	static_cast<zU8>(zenOffsetOf(&SimpleVertex::Pos)) ),
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,			static_cast<zU8>(zenOffsetOf(&SimpleVertex::Tex)) ) 
};

bool SampleDebugUIInstance::IsDone()
{
	return false;
}

const char* SampleDebugUIInstance::GetAppName() const
{
	return "Sample UI";
}

bool SampleDebugUIInstance::Init()
{
	Super::Init();
	CreateGfxWindow( zVec2U16(1280, 800), zVec2U16(0,0) );
	
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
	};

	zArrayStatic<const zenRes::zShaderParameter*> aParamAll = { &zenRes::zShaderFloat4(zHash32("vMeshColor"),	zVec4F(.7f,.7f,.7f,1)),
																&zenRes::zShaderFloat4(zHash32("vColor"),		zVec4F(1.f,1.f,1.f,1.f)) };

	//---------------------------------------------------------------------
	// Create rendering resources		
	//---------------------------------------------------------------------	
	mrCubeVertex									= zenRes::zGfxVertex::Create(aVerticeStreams, zFlagResUse());	
	mrCubeIndex										= zenRes::zGfxIndex::Create( aCubeIndices, zenConst::kePrimType_TriangleList );
	mrShaderVS										= zenRes::zGfxShaderVertex::Create( "Shader/Tutorial07.fx", "VS");
	mrShaderPS										= zenRes::zGfxShaderPixel::Create( "Shader/Tutorial07.fx", "PS" );
	mrTexture										= zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	mrSampler										= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Trilinear, zenConst::keTexFilter_Trilinear, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	
	// Some bindings of render resource together
	mrShaderBind									= zenRes::zGfxShaderBinding::Create(mrShaderVS, mrShaderPS);
	mrCubeMeshStrip									= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrShaderBind );
	mrStateRaster									= zenRes::zGfxStateRaster::Create(zenRes::zGfxStateRaster::Config());
		
	//-------------------------------------------------
	// Init some shader values
	//---------------------------------------------------------------------
	zenMath::V4 vEye	= zenMath::simdXYZW( 0.0f, 0.0f,-5.0f, 0.0f );
	zenMath::V4 vAt		= zenMath::simdXYZW( 0.0f, 0.0f, 0.0f, 0.0f );
	zenMath::V4 vUp		= zenMath::simdXYZW( 0.0f, 1.0f, 0.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );		
	matWorld.SetIdentity();
	matWorld.SetPos(zenMath::simdXYZW(3.0f, 0.0f, 0.0f, 1.0f));
	mrCubeMeshStrip.SetValue( aParamAll );
	mrCubeMeshStrip.SetValue( zHash32("View"),		matView );
	mrCubeMeshStrip.SetValue( zHash32("Projection"),matProjection );		
	mrCubeMeshStrip.SetValue( zHash32("World"),		matWorld );
	mrCubeMeshStrip.SetValue( zHash32("txColor"),	mrTexture, mrSampler);
	mrCubeMeshStrip.SetValue( zHash32("vColor"),	zVec4F(1,1,1,1));

	return true;
}

void SampleDebugUIInstance::UpdateBackbuffers()
{
	zenPerf::zScopedEventCpu EmitEvent("UpdateBackbuffers");
	if( !mrRndPassFinal.IsValid() || mrMainWindowGfx.PerformResize() )
	{	
		zVec2U16 vBackbufferDim = mrMainWindowGfx.GetBackbuffer().GetDim();
		{	
			zenRes::zGfxRenderPass::ConfigColorRT	FinalColorRTConfig;
			zenRes::zGfxRenderPass::ConfigDepthRT	FinalDepthRTConfig;					
			mrBackbufferDepth						= zenRes::zGfxTarget2D::Create(zenConst::keTexFormat_D24S8, vBackbufferDim ); 
			FinalColorRTConfig.mrTargetSurface		= mrMainWindowGfx.GetBackbuffer();
			FinalDepthRTConfig.mrTargetSurface		= mrBackbufferDepth;
			FinalDepthRTConfig.mbDepthEnable		= true;
			FinalDepthRTConfig.mbDepthWrite			= true;
			FinalDepthRTConfig.meDepthTest			= zenRes::zGfxRenderPass::ConfigDepthRT::keCmpTest_Less;
			mrRndPassFinal							= zenRes::zGfxRenderPass::Create("RenderBackbufferFinal", 2, FinalColorRTConfig, FinalDepthRTConfig, mrStateRaster);	
			zenMath::MatrixProjectionLH( matProjection, 60, float(vBackbufferDim.y)/float(vBackbufferDim.x), 0.01f, 100.f );
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
	zenPerf::zScopedEventCpu EmitEvent("SampleDebugUIInstance::Update");

	UpdateBackbuffers();

	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------
	{		
		mrMainWindowGfx.FrameBegin();						
		{			
			zenPerf::zScopedEventCpu EmitEvent("Draw");
			zenGfx::zContext rCxtRender	= zenGfx::zContext::Create("RenderLoop", mrRndPassFinal);
			
			float t				= static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation
			zVec4F vClearColor	= zenMath::TriLerp( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
			matWorld.SetRotationY( t );	// Rotate cube around the origin 				
			mrCubeMeshStrip.SetValue( zHash32("World"), matWorld );
			mrCubeMeshStrip.SetValue( zHash32("Projection"), matProjection );

			zenGfx::zCommand::ClearColor(rCxtRender, mrMainWindowGfx.GetBackbuffer(), vClearColor);
			zenGfx::zCommand::ClearDepthStencil(rCxtRender, mrBackbufferDepth);
			zenGfx::zCommand::DrawMesh(rCxtRender, 0, mrCubeMeshStrip);
			rCxtRender.Submit();		
		}		
		mrMainWindowGfx.FrameEnd();
	}

}
}
