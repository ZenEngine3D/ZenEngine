#include "zenEngine.h"
#include "SampleDirectX12.h"

int main (int argc, char * const argv[])
{
	sample::SampleDirextX12Instance EngineInstance;
	zenSys::LaunchEngine( &EngineInstance, argc, argv );	
	return 0;	
}

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================
namespace sample
{

const zArrayStatic<zVec3F> aCubeVerticesPos =
{
	zVec3F( -1.0f, 1.0f, -1.0f ),	zVec3F( 1.0f, 1.0f, -1.0f ),	zVec3F( 1.0f, 1.0f, 1.0f ),		zVec3F( -1.0f, 1.0f, 1.0f ),
	zVec3F( -1.0f, -1.0f, -1.0f ),	zVec3F( 1.0f, -1.0f, -1.0f ),	zVec3F( 1.0f, -1.0f, 1.0f ),	zVec3F( -1.0f, -1.0f, 1.0f ),
	zVec3F( -1.0f, -1.0f, 1.0f ),	zVec3F( -1.0f, -1.0f, -1.0f ),	zVec3F( -1.0f, 1.0f, -1.0f ),	zVec3F( -1.0f, 1.0f, 1.0f ),
	zVec3F( 1.0f, -1.0f, 1.0f ),	zVec3F( 1.0f, -1.0f, -1.0f ),	zVec3F( 1.0f, 1.0f, -1.0f ),	zVec3F( 1.0f, 1.0f, 1.0f ),
	zVec3F( -1.0f, -1.0f, -1.0f ),	zVec3F( 1.0f, -1.0f, -1.0f ),	zVec3F( 1.0f, 1.0f, -1.0f ),	zVec3F( -1.0f, 1.0f, -1.0f ),
	zVec3F( -1.0f, -1.0f, 1.0f ),	zVec3F( 1.0f, -1.0f, 1.0f ),	zVec3F( 1.0f, 1.0f, 1.0f ),		zVec3F( -1.0f, 1.0f, 1.0f ),
};

const zArrayStatic<BufferColorUV> aCubeVerticesColorUV =
{
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 0
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 1
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 2
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 3
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 4
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 1.0f ) }, //Face 5
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 1.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 1.0f, 0.0f ) },
	{ zVec4U8( 0xFF, 0xFF, 0xFF, 0xFF ), zVec2F( 0.0f, 0.0f ) },
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

bool SampleDirextX12Instance::IsDone()
{
	return false;
}

const char* SampleDirextX12Instance::GetAppName() const
{
	return "Sample DirectX12";
}

bool SampleDirextX12Instance::Init()
{
	Super::Init();
	CreateGfxWindow( zVec2U16(1280, 800), zVec2U16(0,0) );

	//-----------------------------------------------------------
	// Prepare some data for asset creation
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

	//---------------------------------------------------------------------
	// Create rendering resources		
	//---------------------------------------------------------------------	
	mrCubeIndex										= zenRes::zGfxIndex::Create( aCubeIndices, zenConst::kePrimType_TriangleList );
	mrVertexBufferPos								= zenRes::zGfxStructBuffer<zVec3F>::Create(aCubeVerticesPos, (zU32)aCubeVerticesPos.Count() /*, zFlagResUse()*/ ); 
	mrVertexBufferColorUv							= zenRes::zGfxStructBuffer<BufferColorUV>::Create(aCubeVerticesColorUV, (zU32)aCubeVerticesColorUV.Count() /*, zFlagResUse()*/ );

	mrShaderVS										= zenRes::zGfxShaderVertex::Create( "Shader/DX12Sample.sl", "VSMain");
	mrShaderPS										= zenRes::zGfxShaderPixel::Create( "Shader/DX12Sample.sl", "PSMain" );
	//mrTexture										= zenRes::zGfxTexture2D::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	//mrSampler										= zenRes::zGfxStateSampler::Create(zenConst::keTexFilter_Trilinear, zenConst::keTexFilter_Trilinear, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	
	// Some bindings of render resource together
	mrShaderBind									= zenRes::zGfxShaderBinding::Create(mrShaderVS, mrShaderPS);
	mrCubeMeshStrip									= zenRes::zGfxMeshStrip::Create( mrCubeIndex, mrShaderBind );
	mrStateRaster									= zenRes::zGfxStateRaster::Create(zenRes::zGfxStateRaster::Config());
		
	//-------------------------------------------------
	// Init some shader values
	//---------------------------------------------------------------------
	//mrCubeMeshStrip.SetResource(	zHash32("g_texture"),	mrTexture);
	//mrCubeMeshStrip.SetResource(	zHash32("g_sampler"),	mrSampler);	
	
	return true;
}

void SampleDirextX12Instance::UpdateBackbuffers()
{
	zenPerf::zScopedEventCpu EmitEvent("UpdateBackbuffers");
	if( !mrBackbufferDepth.IsValid() || mrMainWindowGfx.PerformResize() )
	{	
		zVec2U16 vBackbufferDim		= mrMainWindowGfx.GetBackbuffer().GetDim();
		mrBackbufferDepth			= zenRes::zGfxTarget2D::Create(zenConst::keTexFormat_D32, vBackbufferDim ); 
		//zenMath::MatrixProjectionLH( matProjection, 60, float(vBackbufferDim.y)/float(vBackbufferDim.x), 0.01f, 100.f );
	}
	
	// Recreates final RenderPass each frame, since backbuffer Target2d gets pingpong-ed
	zenRes::zGfxRenderPass::ConfigColorRT	FinalColorRTConfig;
	zenRes::zGfxRenderPass::ConfigDepthRT	FinalDepthRTConfig;								
	FinalColorRTConfig.mrTargetSurface		= mrMainWindowGfx.GetBackbuffer();
	FinalDepthRTConfig.mrTargetSurface		= mrBackbufferDepth;
	FinalDepthRTConfig.mbDepthEnable		= true;
	FinalDepthRTConfig.mbDepthWrite			= true;
	FinalDepthRTConfig.meDepthTest			= zenRes::zGfxRenderPass::ConfigDepthRT::keCmpTest_Less;
	mrRndPassFinal							= zenRes::zGfxRenderPass::Create("RenderBackbufferFinal", 0, FinalColorRTConfig, FinalDepthRTConfig, mrStateRaster);
}

void SampleDirextX12Instance::Destroy()
{
	Super::Destroy();
}

void SampleDirextX12Instance::Update()
{	
	Super::Update();
	zenPerf::zScopedEventCpu EmitEvent("SampleDirextX12Instance::Update");

	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------
	{		
		mrMainWindowGfx.FrameBegin();
		UpdateBackbuffers();
		{			
			zenPerf::zScopedEventCpu EmitEvent("Draw");
			zenGfx::zContext rCxtRender	= zenGfx::zContext::Create("RenderLoop", mrRndPassFinal);
			
			float t				= static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation
			zVec4F vClearColor	= zenMath::TriLerp( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
			//matWorld.SetRotationY( t );	// Rotate cube around the origin 				
			//mrCubeMeshStrip.SetValue( zHash32("World"), matWorld );
			//mrCubeMeshStrip.SetValue( zHash32("Projection"), matProjection );

			zenGfx::zCommand::ClearColor(rCxtRender, mrMainWindowGfx.GetBackbuffer(), vClearColor);
			zenGfx::zCommand::ClearDepthStencil(rCxtRender, mrBackbufferDepth);
			zenGfx::zCommand::DrawMesh(rCxtRender, 0, mrCubeMeshStrip);
			rCxtRender.Submit();		
		}		
		mrMainWindowGfx.FrameEnd();
	}

}
}
