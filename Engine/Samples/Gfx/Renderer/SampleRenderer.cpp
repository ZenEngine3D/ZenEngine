#include "zenEngine.h"
#include "SampleRenderer.h"

//Test merge

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================
namespace sample
{

SimpleVertex CubeVertices[] =
{
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),		zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },

	{ zVec3F( -1.0f, -1.0f, -1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },

	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( -1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },

	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),		zVec2F( 0.0f, 1.0f ) },

	{ zVec3F( -1.0f, -1.0f, -1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( 1.0f, -1.0f, -1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, -1.0f ),	zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, 1.0f, -1.0f ),	zVec2F( 0.0f, 1.0f ) },

	{ zVec3F( -1.0f, -1.0f, 1.0f ),	zVec2F( 0.0f, 0.0f ) },
	{ zVec3F( 1.0f, -1.0f, 1.0f ),	zVec2F( 1.0f, 0.0f ) },
	{ zVec3F( 1.0f, 1.0f, 1.0f ),		zVec2F( 1.0f, 1.0f ) },
	{ zVec3F( -1.0f, 1.0f, 1.0f ),	zVec2F( 0.0f, 1.0f ) },
};

const zenRes::zGfxVertex::Element CubeVerticeInfos[]={	
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 3, zenConst::keShaderSemantic_Position,	ZENMemberOffset(SimpleVertex, Pos) ),
	zenRes::zGfxVertex::Element(zenConst::keShaderElemType_Float, 2, zenConst::keShaderSemantic_UV,		ZENMemberOffset(SimpleVertex, Tex) ) 
};

zU16 CubeIndices[] =
{
	3,1,0,		2,1,3,
	6,4,5,		7,4,6,
	11,9,8,		10,9,11,
	14,12,13,	15,12,14,
	19,17,16,	18,17,19,
	22,20,21,	23,20,22
};


bool SampleRendererInstance::IsDone()
{
	return false;
}

bool SampleRendererInstance::Init()
{
	Super::Init();
	CreateGfxWindow( zVec2U16(1280, 800), zVec2U16(0,0) );

	//-----------------------------------------------------------
	// Prepare some data for asset creation
	zArrayStatic<zenRes::zGfxVertex::Stream> aVerticeStreams(1);		
	aVerticeStreams[0].muStride = sizeof(SimpleVertex);
	aVerticeStreams[0].maData.Copy( (zU8*)CubeVertices, sizeof(CubeVertices) );
	aVerticeStreams[0].maElements.Copy(CubeVerticeInfos, ZENArrayCount(CubeVerticeInfos) );

	zArrayStatic<zU8>		aTexRGBA;
	zVec2U16				vTexSize(256,256);
	zenConst::eTextureFormat	eTexFormat = zenConst::keTexFormat_RGBA8;
	//aTexRGBA.SetCount( EExp::GetTextureBlockInfo(eTexFormat).muSize * vTexSize.x * vTexSize.y ); //! @todo Missing: export this in api
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

	const zenRes::zShaderParameter* ParamAll[] =
	{
		&zenRes::zShaderFloat4(zHash32("vMeshColor"),	zVec4F(.7f,.7f,.7f,1)),
		&zenRes::zShaderFloat4(zHash32("vColor"),		zVec4F(1,1,1,1)),
	};
	zArrayStatic<const zenRes::zShaderParameter*>	aParamAll( ParamAll, ZENArrayCount(ParamAll));

	//---------------------------------------------------------------------
	// Create rendering resources		
	//---------------------------------------------------------------------
	zArrayStatic<zenRes::zShaderDefine> test(2);
	test[0] = zenRes::zShaderDefine("DEFINETEST", "1");

	mrCubeVertex			= zenRes::zGfxVertex::Create(aVerticeStreams, zFlagResUse());	
	mrCubeIndex			= zenRes::zGfxIndex::Create( zArrayStatic<zU16>(CubeIndices, ZENArrayCount(CubeIndices), TRUE), zenConst::kePrimType_TriangleList );
	mrShaderVS			= zenRes::zGfxShaderVertex::Create( "Shader/Tutorial07.fx", "VS");
	mrShaderPS			= zenRes::zGfxShaderPixel::Create( "Shader/Tutorial07.fx", "PS", test );		
	mrTexture			= zenRes::zGfxTexture2d::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	mrSampler			= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Trilinear, zenConst::keTexFilter_Bilinear, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrSampler2			= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	mrRenderColor		= zenRes::zGfxRenderTarget::Create(zenConst::keTexFormat_RGBA8, zVec2U16(512,512) );
	mrRenderDepth		= zenRes::zGfxRenderTarget::Create(zenConst::keTexFormat_D24S8, zVec2U16(512,512) );
	mrView				= zenRes::zGfxView::Create( mrRenderColor, mrRenderDepth, zVec2U16(256,256), zVec2S16(256,256) );

	// Some bindings of render resource together
	const zenRes::zGfxShader pShaders[]			= {mrShaderVS, mrShaderPS};
	mrTestShaderBind	= zenRes::zGfxShaderBinding::Create( pShaders, ZENArrayCount(pShaders) );
	mrCube1MeshStrip	= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrTestShaderBind );
	mrCube2MeshStrip	= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrTestShaderBind );
	mrCube3MeshStrip	= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrTestShaderBind );

	zenRes::zGfxMeshStrip rCube4MeshStripA		= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrTestShaderBind, 0, 12 );
	zenRes::zGfxMeshStrip rCube4MeshStripB		= zenRes::zGfxMeshStrip::Create( mrCubeVertex, mrCubeIndex, mrTestShaderBind, 12, 0xFFFFFFFF );
	const zenRes::zGfxMeshStrip aMesh4Strip[]	= {rCube4MeshStripA, rCube4MeshStripB};
	mrCube4Mesh									= zenRes::zGfxMesh::Create( aMesh4Strip, ZENArrayCount(aMesh4Strip) );

	mrStateBlend		= zenRes::zGfxStateBlend::Create();
	mrStateDepth		= zenRes::zGfxStateDepthStencil::Create(true, true);
	mrStateRaster		= zenRes::zGfxStateRasterizer::Create(false, false, false, false, false, false, zenConst::eCullMode::keCullMode_Back);
	mrRndPassTexture	= zenRes::zGfxRenderPass::Create("RenderToTexture",mrStateBlend,mrStateDepth,mrStateRaster,mrView);
	mrRndPassFinal		= zenRes::zGfxRenderPass::Create("RenderBackbufferFinal",mrStateBlend,mrStateDepth,mrStateRaster,mrMainGfxWindow.GetBackbuffer());

	//-------------------------------------------------
	// Init some shader values
	//---------------------------------------------------------------------
	for(int i=0; i<ZENArrayCount(matWorld); ++i)
		matWorld[i].SetIdentity();

	zenMath::V4 vEye	= zenMath::simdXYZW( 0.0f, 0.0f, -2.0f, 0.0f );
	zenMath::V4 vAt		= zenMath::simdXYZW( 0.0f, 0.0f, 0.0f, 0.0f );
	zenMath::V4 vUp		= zenMath::simdXYZW( 0.0f, 1.0f, 0.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );
	zenMath::MatrixProjectionLH( matProjectionRT, 60, float(mrRenderColor.GetDim().y)/float(mrRenderColor.GetDim().x), 0.01f, 100.f );
	mrCube1MeshStrip.SetValue( aParamAll );
	mrCube1MeshStrip.SetValue( zHash32("World"),			matWorld[0] );
	mrCube1MeshStrip.SetValue( zHash32("View"),			matView );
	mrCube1MeshStrip.SetValue( zHash32("Projection"),	matProjectionRT );
	mrCube1MeshStrip.SetValue( zHash32("txColor"),		mrTexture, mrSampler);

	vEye	= zenMath::simdXYZW( 0.0f, 0.0f, -5.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );		
	matWorld[1].SetPos(zenMath::simdXYZW(-3.0f, 0.0f, 0.0f, 1.0f));
	matWorld[1].SetRotationY( zenMath::kfPI2x );		
	mrCube2MeshStrip.SetValue( aParamAll );		
	mrCube2MeshStrip.SetValue( zHash32("View"),		matView );				
	mrCube2MeshStrip.SetValue( zHash32("World"),	matWorld[1] );
	mrCube2MeshStrip.SetValue( zHash32("txColor"),	mrRenderColor.GetTexture2D(), mrSampler);
	mrCube2MeshStrip.SetValue( zHash32("vColor"),	zVec4F(1,0,1,1));

	matWorld[2].SetPos(zenMath::simdXYZW(3.0f, 0.0f, 0.0f, 1.0f));
	mrCube3MeshStrip.SetValue( aParamAll );
	mrCube3MeshStrip.SetValue( zHash32("View"),		matView );
	mrCube3MeshStrip.SetValue( zHash32("Projection"),matProjection );		
	mrCube3MeshStrip.SetValue( zHash32("World"),		matWorld[2] );
	mrCube3MeshStrip.SetValue( zHash32("txColor"),	mrTexture, mrSampler2);
	mrCube3MeshStrip.SetValue( zHash32("vColor"),	zVec4F(1,1,1,1));

	matWorld[3].SetPos(zenMath::simdXYZW(0.0f, 0.0f, 0.0f, 1.0f));
	mrCube4Mesh.SetValue(aParamAll);
	mrCube4Mesh.SetValue( zHash32("View"),			matView );
	mrCube4Mesh.SetValue( zHash32("Projection"),	matProjection );		
	mrCube4Mesh.SetValue( zHash32("World"),			matWorld[3] );
	mrCube4Mesh.SetValue( zHash32("txColor"),		mrTexture, mrSampler);
	rCube4MeshStripA.SetValue( zHash32("vColor"),	zVec4F(1,0.2f,0.2f,1));
	rCube4MeshStripB.SetValue( zHash32("vColor"),	zVec4F(0.2f,1,0.2f,1));	
	
	//---------------------------------------------------------------------
	// Testing Lock
	//---------------------------------------------------------------------
	SimpleVertex* pVertexSrc	= CubeVertices;
	SimpleVertex* pVertexSrcEnd	= &CubeVertices[ZENArrayCount(CubeVertices)];
	SimpleVertex* pVertex		= (SimpleVertex*)mrCubeVertex.Lock();		
	while( pVertexSrc < pVertexSrcEnd )
	{
		pVertex->Pos = pVertexSrc->Pos * 1.5;
		pVertex->Tex = pVertexSrc->Tex;
		++pVertexSrc;
		++pVertex;
	}
	mrCubeVertex.Unlock();
	return true;
}

void SampleRendererInstance::Destroy()
{
	Super::Destroy();
}

void SampleRendererInstance::Update()
{	
	Super::Update();
	zArrayDynamic<zenRes::zGfxDrawcall> aDrawcalls(128);

	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------
	static zVec2U16 vLastSize = mrMainGfxWindow.GetBackbuffer().GetDim();
	zenMath::MatrixProjectionLH( matProjection, 60, float(vLastSize.y)/float(vLastSize.x), 0.01f, 100.f );			
	if( vLastSize != mrMainGfxWindow.GetBackbuffer().GetDim() )
	{
		vLastSize = mrMainGfxWindow.GetBackbuffer().GetDim();
		mrMainGfxWindow.Resize(vLastSize);
		zenMath::MatrixProjectionLH( matProjection, 60, float(vLastSize.y)/float(vLastSize.x), 0.01f, 100.f );
	}

	mrMainGfxWindow.FrameBegin();
	float t = static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation

	//-----------------------------------------------------------------
	// Render cube in RenderTarget
	//-----------------------------------------------------------------
	{				
		mrView.Clear( true, zVec4F(1,0,0,0) );
		zVec4F vShaderColor = zenMath::TriLerp<zVec4F>( zVec4F(1,1,1,1), zVec4F(0.15f,0.15f,1.0f,1), zVec4F(1,1,1,1), zenMath::Fract(t*2) );
		matWorld[0].SetRotationY( t );							// Rotate cube around the origin
		mrCube1MeshStrip.SetValue( zHash32("World"),	matWorld[0] );
		mrCube1MeshStrip.SetValue( zHash32("vColor"),	vShaderColor);
		mrCube1MeshStrip.Draw(mrRndPassTexture, 0, aDrawcalls);
	}

	//-----------------------------------------------------------------
	// Render cubes in main render target
	//-----------------------------------------------------------------
	zVec4F vClearColor = zenMath::TriLerp( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
	mrMainGfxWindow.GetBackbuffer().Clear( true, vClearColor, true, 0 );

	// Render the cube with rendertarget as texture
	mrCube2MeshStrip.SetValue( zHash32("World"),		matWorld[1] );
	mrCube2MeshStrip.SetValue( zHash32("Projection"),	matProjection );
	mrCube2MeshStrip.Draw(mrRndPassFinal, 0, aDrawcalls);

	// Render the cube with point sampling
	matWorld[2].SetRotationY( t );						// Rotate cube around the origin 				
	mrCube3MeshStrip.SetValue( zHash32("World"),		matWorld[2] );
	mrCube3MeshStrip.SetValue( zHash32("Projection"),	matProjection );
	mrCube3MeshStrip.Draw(mrRndPassFinal, 0, aDrawcalls);

	matWorld[3].SetRotationX( t );						// Rotate cube around the origin 				
	mrCube4Mesh.SetValue( zHash32("World"),				matWorld[3] );
	mrCube4Mesh.SetValue( zHash32("Projection"),		matProjection );
	mrCube4Mesh.SetValue( zHash32("World"),				matWorld[3] );
	mrCube4Mesh.SetValue( zHash32("Projection"),		matProjection );	
	mrCube4Mesh.Draw(mrRndPassFinal, 0, aDrawcalls);
	
	zenRes::zGfxDrawcall::Submit(aDrawcalls);

	mrMainGfxWindow.FrameEnd();
	
}
}
