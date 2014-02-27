#include "zenEngine.h"
#include "SampleRenderer.h"

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================
namespace sample
{

SimpleVertex CubeVertices[] =
{
	{ zenVec3F( -1.0f, 1.0f, -1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( 1.0f, 1.0f, -1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( 1.0f, 1.0f, 1.0f ),		zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( -1.0f, 1.0f, 1.0f ),	zenVec2F( 0.0f, 1.0f ) },

	{ zenVec3F( -1.0f, -1.0f, -1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( 1.0f, -1.0f, -1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( 1.0f, -1.0f, 1.0f ),	zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( -1.0f, -1.0f, 1.0f ),	zenVec2F( 0.0f, 1.0f ) },

	{ zenVec3F( -1.0f, -1.0f, 1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( -1.0f, -1.0f, -1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( -1.0f, 1.0f, -1.0f ),	zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( -1.0f, 1.0f, 1.0f ),	zenVec2F( 0.0f, 1.0f ) },

	{ zenVec3F( 1.0f, -1.0f, 1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( 1.0f, -1.0f, -1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( 1.0f, 1.0f, -1.0f ),	zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( 1.0f, 1.0f, 1.0f ),		zenVec2F( 0.0f, 1.0f ) },

	{ zenVec3F( -1.0f, -1.0f, -1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( 1.0f, -1.0f, -1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( 1.0f, 1.0f, -1.0f ),	zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( -1.0f, 1.0f, -1.0f ),	zenVec2F( 0.0f, 1.0f ) },

	{ zenVec3F( -1.0f, -1.0f, 1.0f ),	zenVec2F( 0.0f, 0.0f ) },
	{ zenVec3F( 1.0f, -1.0f, 1.0f ),	zenVec2F( 1.0f, 0.0f ) },
	{ zenVec3F( 1.0f, 1.0f, 1.0f ),		zenVec2F( 1.0f, 1.0f ) },
	{ zenVec3F( -1.0f, 1.0f, 1.0f ),	zenVec2F( 0.0f, 1.0f ) },
};

const zenRes::GfxVertex::Element CubeVerticeInfos[]={	
	zenRes::GfxVertex::Element(awconst::keShaderElemType_Float, 3, awconst::keShaderSemantic_Position,	ZENMemberOffset(SimpleVertex, Pos) ),
	zenRes::GfxVertex::Element(awconst::keShaderElemType_Float, 2, awconst::keShaderSemantic_UV,		ZENMemberOffset(SimpleVertex, Tex) ) 
};

zenU16 CubeIndices[] =
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
	CreateGfxWindow( zenVec2U16(1280, 800), zenVec2U16(0,0) );

	//-----------------------------------------------------------
	// Prepare some data for asset creation
	zenArrayStatic<zenRes::GfxVertex::Stream> aVerticeStreams(1);		
	aVerticeStreams[0].muStride = sizeof(SimpleVertex);
	aVerticeStreams[0].maData.Copy( (zenU8*)CubeVertices, sizeof(CubeVertices) );
	aVerticeStreams[0].maElements.Copy(CubeVerticeInfos, ZENArrayCount(CubeVerticeInfos) );

	zenArrayStatic<zenU8>		aTexRGBA;
	zenVec2U16				vTexSize(256,256);
	awconst::eTextureFormat	eTexFormat = awconst::keTexFormat_RGBA8;
	//aTexRGBA.SetCount( EExp::GetTextureBlockInfo(eTexFormat).muSize * vTexSize.x * vTexSize.y ); //! @todo export this in api
	aTexRGBA.SetCount( vTexSize.x*vTexSize.y*4 );
	zenU8*						pTexCur = aTexRGBA.First();
	for(zenUInt line=0; line<vTexSize.y; ++line)
	{
		for(zenUInt col=0; col<vTexSize.x; ++col)
		{
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0x10;
			*pTexCur++ = (line/16+col/16) % 2 == 0 ? 0xFF : 0xFF;
			*pTexCur++ = 1;
		}
	}

	const zenRes::awShaderParameter* ParamAll[] =
	{
		&zenRes::awShaderFloat4(zenHash32("vMeshColor"),	zenVec4F(.7f,.7f,.7f,1)),
		&zenRes::awShaderFloat4(zenHash32("vColor"),		zenVec4F(1,1,1,1)),
	};
	zenArrayStatic<const zenRes::awShaderParameter*>	aParamAll( ParamAll, ZENArrayCount(ParamAll));

	//---------------------------------------------------------------------
	// Create rendering resources		
	//---------------------------------------------------------------------
	zenArrayStatic<zenRes::awShaderDefine> test(2);
	test[0] = zenRes::awShaderDefine("DEFINETEST", "1");

	rCubeVertex			= zenRes::GfxVertex::Create(aVerticeStreams, 0);
	rCubeIndex			= zenRes::GfxIndex::Create( zenArrayStatic<zenU16>(CubeIndices, ZENArrayCount(CubeIndices), TRUE), awconst::kePrimType_TriangleList );
	rShaderVS			= zenRes::GfxShaderVertex::Create( "Shader/Tutorial07.fx", "VS");
	rShaderPS			= zenRes::GfxShaderPixel::Create( "Shader/Tutorial07.fx", "PS", test );		
	rTexture			= zenRes::GfxTexture2D::Create(awconst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	rSampler			= zenRes::GfxSampler::Create(awconst::keTexFilter_Trilinear, awconst::keTexFilter_Bilinear, awconst::keTexWrap_Clamp, awconst::keTexWrap_Clamp, 0);
	rSampler2			= zenRes::GfxSampler::Create(awconst::keTexFilter_Point, awconst::keTexFilter_Point, awconst::keTexWrap_Clamp, awconst::keTexWrap_Clamp, 0);
	rRenderColor		= zenRes::GfxRenderTarget::Create(awconst::keTexFormat_RGBA8, zenVec2U16(512,512) );
	rRenderDepth		= zenRes::GfxRenderTarget::Create(awconst::keTexFormat_D24S8, zenVec2U16(512,512) );
	rView				= zenRes::GfxView::Create( rRenderColor, rRenderDepth, zenVec2U16(256,256), zenVec2U16(256,256) );

	// Some bindings of render resource together
	const zenRes::GfxShader pShaders[]			= {rShaderVS, rShaderPS};
	rTestShaderBind		= zenRes::GfxShaderBinding::Create( pShaders, ZENArrayCount(pShaders) );
	rCube1MeshStrip		= zenRes::GfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );
	rCube2MeshStrip		= zenRes::GfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );
	rCube3MeshStrip		= zenRes::GfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );

	zenRes::GfxMeshStrip rCube4MeshStripA		= zenRes::GfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind, 0, 12 );
	zenRes::GfxMeshStrip rCube4MeshStripB		= zenRes::GfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind, 12, 0xFFFFFFFF );
	const zenRes::GfxMeshStrip aMesh4Strip[]	= {rCube4MeshStripA, rCube4MeshStripB};
	rCube4Mesh									= zenRes::GfxMesh::Create( aMesh4Strip, ZENArrayCount(aMesh4Strip) );

	//-------------------------------------------------
	// Init some shader values
	//---------------------------------------------------------------------
	for(int i=0; i<ZENArrayCount(matWorld); ++i)
		matWorld[i].SetIdentity();

	zenMath::V4 vEye	= zenMath::XYZW( 0.0f, 0.0f, -2.0f, 0.0f );
	zenMath::V4 vAt	= zenMath::XYZW( 0.0f, 0.0f, 0.0f, 0.0f );
	zenMath::V4 vUp	= zenMath::XYZW( 0.0f, 1.0f, 0.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );
	zenMath::MatrixProjectionLH( matProjectionRT, 60, float(rRenderColor.GetDim().y)/float(rRenderColor.GetDim().x), 0.01f, 100.f );
	rCube1MeshStrip.SetValue( aParamAll );
	rCube1MeshStrip.SetValue( zenHash32("World"),		matWorld[0] );
	rCube1MeshStrip.SetValue( zenHash32("View"),		matView );
	rCube1MeshStrip.SetValue( zenHash32("Projection"),	matProjectionRT );
	rCube1MeshStrip.SetValue( zenHash32("txColor"),		rTexture, rSampler);

	vEye	= zenMath::XYZW( 0.0f, 0.0f, -5.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );		
	matWorld[1].SetPos(zenMath::XYZW(-3.0f, 0.0f, 0.0f, 1.0f));
	matWorld[1].SetRotationY( zenMath::kfPI2x );		
	rCube2MeshStrip.SetValue( aParamAll );		
	rCube2MeshStrip.SetValue( zenHash32("View"),		matView );				
	rCube2MeshStrip.SetValue( zenHash32("World"),		matWorld[1] );
	rCube2MeshStrip.SetValue( zenHash32("txColor"),		rRenderColor.GetTexture2D(), rSampler);
	rCube2MeshStrip.SetValue( zenHash32("vColor"),		zenVec4F(1,0,1,1));

	matWorld[2].SetPos(zenMath::XYZW(3.0f, 0.0f, 0.0f, 1.0f));
	rCube3MeshStrip.SetValue( aParamAll );
	rCube3MeshStrip.SetValue( zenHash32("View"),		matView );
	rCube3MeshStrip.SetValue( zenHash32("Projection"),	matProjection );		
	rCube3MeshStrip.SetValue( zenHash32("World"),		matWorld[2] );
	rCube3MeshStrip.SetValue( zenHash32("txColor"),		rTexture, rSampler2);
	rCube3MeshStrip.SetValue( zenHash32("vColor"),		zenVec4F(1,1,1,1));


	matWorld[3].SetPos(zenMath::XYZW(0.0f, 0.0f, 0.0f, 1.0f));
	rCube4Mesh.SetValue(aParamAll);
	rCube4Mesh.SetValue( zenHash32("View"),				matView );
	rCube4Mesh.SetValue( zenHash32("Projection"),		matProjection );		
	rCube4Mesh.SetValue( zenHash32("World"),			matWorld[3] );
	rCube4Mesh.SetValue( zenHash32("txColor"),			rTexture, rSampler);
	rCube4MeshStripA.SetValue( zenHash32("vColor"),		zenVec4F(1,0.2f,0.2f,1));
	rCube4MeshStripB.SetValue( zenHash32("vColor"),		zenVec4F(0.2f,1,0.2f,1));	


	//---------------------------------------------------------------------
	// Testing Lock
	//---------------------------------------------------------------------
	SimpleVertex* pVertexSrc	= CubeVertices;
	SimpleVertex* pVertexSrcEnd	= &CubeVertices[ZENArrayCount(CubeVertices)];
	SimpleVertex* pVertex		= (SimpleVertex*)rCubeVertex.Lock();		
	while( pVertexSrc < pVertexSrcEnd )
	{
		pVertex->Pos = pVertexSrc->Pos * 1.5;
		pVertex->Tex = pVertexSrc->Tex;
		++pVertexSrc;
		++pVertex;
	}
	rCubeVertex.Unlock();
	return true;
}

void SampleRendererInstance::Destroy()
{
	Super::Destroy();
}

void SampleRendererInstance::Update()
{	
	Super::Update();
	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------
	static zenVec2U16 vLastSize = mrMainGfxWindow.GetBackbuffer().GetDim();
	zenMath::MatrixProjectionLH( matProjection, 60, float(vLastSize.y)/float(vLastSize.x), 0.01f, 100.f );			
	if( vLastSize != mrMainGfxWindow.GetBackbuffer().GetDim() )
	{
		vLastSize = mrMainGfxWindow.GetBackbuffer().GetDim();
		mrMainGfxWindow.Resize(vLastSize);
		zenMath::MatrixProjectionLH( matProjection, 60, float(vLastSize.y)/float(vLastSize.x), 0.01f, 100.f );
	}

	mrMainGfxWindow.FrameBegin();
	static float t(0);
	t += 0.01f; //! @todo Get elapsed time back
	//float t = static_cast<float>(CSys::GetElapsedSec() / 3.0);	// Update our time animation

	//-----------------------------------------------------------------
	// Render cube in rendertarget
	//-----------------------------------------------------------------
	{				
		rView.ActivateView();
		rView.Clear( true, zenVec4F(1,0,0,0) );
		zenVec4F vShaderColor = zenMath::TriLerp<zenVec4F>( zenVec4F(1,1,1,1), zenVec4F(0.15f,0.15f,1.0f,1), zenVec4F(1,1,1,1), zenMath::Fract(t*2) );
		matWorld[0].SetRotationY( t );							// Rotate cube around the origin
		rCube1MeshStrip.SetValue( zenHash32("World"),		matWorld[0] );
		rCube1MeshStrip.SetValue( zenHash32("vColor"),		vShaderColor);
		rCube1MeshStrip.RenderMeshStrip();
	}

	//-----------------------------------------------------------------
	// Render cubes in main render target
	//-----------------------------------------------------------------
	mrMainGfxWindow.GetBackbuffer().ActivateView();
	zenVec4F vClearColor = zenMath::TriLerp<zenVec4F>( zenVec4F(0.05f,0.05f,0.05f,1), zenVec4F(0.1f,0.1f,0.20f,1), zenVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
	mrMainGfxWindow.GetBackbuffer().Clear( true, vClearColor, true, 0 );


	// Render the cube with rendertarget as texture
	rCube2MeshStrip.SetValue( zenHash32("World"),			matWorld[1] );
	rCube2MeshStrip.SetValue( zenHash32("Projection"),		matProjection );
	rCube2MeshStrip.RenderMeshStrip();

	// Render the cube with point sampling
	matWorld[2].SetRotationY( t );							// Rotate cube around the origin 				
	rCube3MeshStrip.SetValue( zenHash32("World"),			matWorld[2] );
	rCube3MeshStrip.SetValue( zenHash32("Projection"),		matProjection );
	rCube3MeshStrip.RenderMeshStrip();

	matWorld[3].SetRotationX( t );							// Rotate cube around the origin 				
	rCube4Mesh.SetValue( zenHash32("World"),					matWorld[3] );
	rCube4Mesh.SetValue( zenHash32("Projection"),			matProjection );
	rCube4Mesh.SetValue( zenHash32("World"),					matWorld[3] );
	rCube4Mesh.SetValue( zenHash32("Projection"),			matProjection );
	rCube4Mesh.RenderMesh();

	mrMainGfxWindow.FrameEnd();
	
}
}
