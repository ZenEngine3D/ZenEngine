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

	rCubeVertex			= zenRes::zGfxVertex::Create(aVerticeStreams, zFlagResUse());	
	rCubeIndex			= zenRes::zGfxIndex::Create( zArrayStatic<zU16>(CubeIndices, ZENArrayCount(CubeIndices), TRUE), zenConst::kePrimType_TriangleList );
	rShaderVS			= zenRes::zGfxShaderVertex::Create( "Shader/Tutorial07.fx", "VS");
	rShaderPS			= zenRes::zGfxShaderPixel::Create( "Shader/Tutorial07.fx", "PS", test );		
	rTexture			= zenRes::zGfxTexture2D::Create(zenConst::keTexFormat_RGBA8, vTexSize, aTexRGBA );
	rSampler			= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Trilinear, zenConst::keTexFilter_Bilinear, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	rSampler2			= zenRes::zGfxSampler::Create(zenConst::keTexFilter_Point, zenConst::keTexFilter_Point, zenConst::keTexWrap_Clamp, zenConst::keTexWrap_Clamp, 0);
	rRenderColor		= zenRes::zGfxRenderTarget::Create(zenConst::keTexFormat_RGBA8, zVec2U16(512,512) );
	rRenderDepth		= zenRes::zGfxRenderTarget::Create(zenConst::keTexFormat_D24S8, zVec2U16(512,512) );
	rView				= zenRes::zGfxView::Create( rRenderColor, rRenderDepth, zVec2U16(256,256), zVec2U16(256,256) );

	// Some bindings of render resource together
	const zenRes::zGfxShader pShaders[]			= {rShaderVS, rShaderPS};
	rTestShaderBind		= zenRes::zGfxShaderBinding::Create( pShaders, ZENArrayCount(pShaders) );
	rCube1MeshStrip		= zenRes::zGfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );
	rCube2MeshStrip		= zenRes::zGfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );
	rCube3MeshStrip		= zenRes::zGfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind );

	zenRes::zGfxMeshStrip rCube4MeshStripA		= zenRes::zGfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind, 0, 12 );
	zenRes::zGfxMeshStrip rCube4MeshStripB		= zenRes::zGfxMeshStrip::Create( rCubeVertex, rCubeIndex, rTestShaderBind, 12, 0xFFFFFFFF );
	const zenRes::zGfxMeshStrip aMesh4Strip[]	= {rCube4MeshStripA, rCube4MeshStripB};
	rCube4Mesh									= zenRes::zGfxMesh::Create( aMesh4Strip, ZENArrayCount(aMesh4Strip) );

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
	rCube1MeshStrip.SetValue( zHash32("World"),		matWorld[0] );
	rCube1MeshStrip.SetValue( zHash32("View"),		matView );
	rCube1MeshStrip.SetValue( zHash32("Projection"),	matProjectionRT );
	rCube1MeshStrip.SetValue( zHash32("txColor"),		rTexture, rSampler);

	vEye	= zenMath::XYZW( 0.0f, 0.0f, -5.0f, 0.0f );
	zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );		
	matWorld[1].SetPos(zenMath::XYZW(-3.0f, 0.0f, 0.0f, 1.0f));
	matWorld[1].SetRotationY( zenMath::kfPI2x );		
	rCube2MeshStrip.SetValue( aParamAll );		
	rCube2MeshStrip.SetValue( zHash32("View"),		matView );				
	rCube2MeshStrip.SetValue( zHash32("World"),		matWorld[1] );
	rCube2MeshStrip.SetValue( zHash32("txColor"),		rRenderColor.GetTexture2D(), rSampler);
	rCube2MeshStrip.SetValue( zHash32("vColor"),		zVec4F(1,0,1,1));

	matWorld[2].SetPos(zenMath::XYZW(3.0f, 0.0f, 0.0f, 1.0f));
	rCube3MeshStrip.SetValue( aParamAll );
	rCube3MeshStrip.SetValue( zHash32("View"),		matView );
	rCube3MeshStrip.SetValue( zHash32("Projection"),	matProjection );		
	rCube3MeshStrip.SetValue( zHash32("World"),		matWorld[2] );
	rCube3MeshStrip.SetValue( zHash32("txColor"),		rTexture, rSampler2);
	rCube3MeshStrip.SetValue( zHash32("vColor"),		zVec4F(1,1,1,1));


	matWorld[3].SetPos(zenMath::XYZW(0.0f, 0.0f, 0.0f, 1.0f));
	rCube4Mesh.SetValue(aParamAll);
	rCube4Mesh.SetValue( zHash32("View"),				matView );
	rCube4Mesh.SetValue( zHash32("Projection"),		matProjection );		
	rCube4Mesh.SetValue( zHash32("World"),			matWorld[3] );
	rCube4Mesh.SetValue( zHash32("txColor"),			rTexture, rSampler);
	rCube4MeshStripA.SetValue( zHash32("vColor"),		zVec4F(1,0.2f,0.2f,1));
	rCube4MeshStripB.SetValue( zHash32("vColor"),		zVec4F(0.2f,1,0.2f,1));	


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
	// Render cube in rendertarget
	//-----------------------------------------------------------------
	{				
		rView.ActivateView();
		rView.Clear( true, zVec4F(1,0,0,0) );
		zVec4F vShaderColor = zenMath::TriLerp<zVec4F>( zVec4F(1,1,1,1), zVec4F(0.15f,0.15f,1.0f,1), zVec4F(1,1,1,1), zenMath::Fract(t*2) );
		matWorld[0].SetRotationY( t );							// Rotate cube around the origin
		rCube1MeshStrip.SetValue( zHash32("World"),		matWorld[0] );
		rCube1MeshStrip.SetValue( zHash32("vColor"),		vShaderColor);
		rCube1MeshStrip.RenderMeshStrip();
	}

	//-----------------------------------------------------------------
	// Render cubes in main render target
	//-----------------------------------------------------------------
	mrMainGfxWindow.GetBackbuffer().ActivateView();
	zVec4F vClearColor = zenMath::TriLerp<zVec4F>( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
	mrMainGfxWindow.GetBackbuffer().Clear( true, vClearColor, true, 0 );


	// Render the cube with rendertarget as texture
	rCube2MeshStrip.SetValue( zHash32("World"),			matWorld[1] );
	rCube2MeshStrip.SetValue( zHash32("Projection"),		matProjection );
	rCube2MeshStrip.RenderMeshStrip();

	// Render the cube with point sampling
	matWorld[2].SetRotationY( t );							// Rotate cube around the origin 				
	rCube3MeshStrip.SetValue( zHash32("World"),			matWorld[2] );
	rCube3MeshStrip.SetValue( zHash32("Projection"),		matProjection );
	rCube3MeshStrip.RenderMeshStrip();

	matWorld[3].SetRotationX( t );							// Rotate cube around the origin 				
	rCube4Mesh.SetValue( zHash32("World"),					matWorld[3] );
	rCube4Mesh.SetValue( zHash32("Projection"),			matProjection );
	rCube4Mesh.SetValue( zHash32("World"),					matWorld[3] );
	rCube4Mesh.SetValue( zHash32("Projection"),			matProjection );
	rCube4Mesh.RenderMesh();

	mrMainGfxWindow.FrameEnd();
	
}
}
