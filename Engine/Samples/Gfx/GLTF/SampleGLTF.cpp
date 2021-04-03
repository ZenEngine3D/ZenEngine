#include "zenEngine.h"
#include "SampleGLTF.h"

//#include "ZenExternal/zxExternal.h" //< Need to rename this to not be engine only lib
#include "ZenExternal/Asset/zxAssLoaderGLTF.h"

int main (int argc, char * const argv[])
{
	sample::SampleGLTLInstance EngineInstance;
	zenSys::LaunchEngine( &EngineInstance, argc, argv );	
	return 0;	
}

//=================================================================================================
//! @example SampleGfxRenderer.cpp
//! Renderer initialization and test
//=================================================================================================
namespace sample
{

bool SampleGLTLInstance::Init()
{
	Super::Init();
	CreateGfxWindow( zVec2U16(1280, 800), zVec2U16(0,0) );
	mrStateRaster = zenRes::zGfxStateRaster::Create(zenRes::zGfxStateRaster::Config());

	mrShaderPS			= zenRes::zGfxShaderPixel::Create( "Shader/GLTFShaders.sl", "PSMain");		
	mrShaderVS			= zenRes::zGfxShaderVertex::Create("Shader/GLTFShaders.sl", "VSMain");	
	mrShaderBind		= zenRes::zGfxShaderBinding::Create(mrShaderVS, mrShaderPS);

	tinygltf::Model		TLModels;
	tinygltf::TinyGLTF	TLContext;
	std::string			Errors, Warnings;
	
	//bool Result = TLContext.LoadBinaryFromFile(&TLModels, &Errors, &Warnings, "E:\\Git/ZenEngine/Build/Content/Scenes/Sponza/glTF/Sponza.gltb");
	bool Result = TLContext.LoadASCIIFromFile(&TLModels, &Errors, &Warnings, "C:\\Git/ZenEngine3D/Build/Content/Scenes/Sponza/glTF/Sponza.gltf");
		
	if (!Warnings.empty())
	{
	  printf("Warn: %s\n", Warnings.c_str());
	}

	// Texture Loading
	zArrayDyn<zU8> aPixels;
	maSceneTexture.resize( TLModels.images.size());
	for(zUInt idxTexture(0), cntTexture(maSceneTexture.size()); idxTexture<cntTexture; ++idxTexture)
	{
		const auto& TFTexture	= TLModels.images[idxTexture];
		auto& Texture			= maSceneTexture[idxTexture];
		zenConst::eTextureFormat eFormat(zenConst::eTextureFormat::keTexFormat__Invalid);
		switch( TFTexture.component )
		{
		case 3: eFormat = zenConst::eTextureFormat::keTexFormat_RGB8;	break;
		case 4: eFormat = zenConst::eTextureFormat::keTexFormat_RGBA8;	break;
		default: zenAssert(0);
		} 
		aPixels.Copy(&TFTexture.image[0], TFTexture.image.size());
		
		Texture.mrTexture	= zenRes::zGfxTexture2D::Create(eFormat, zVec2U16(static_cast<zU16>(TFTexture.width), static_cast<zU16>(TFTexture.height)), aPixels);
		Texture.mName		= TFTexture.name.c_str();
		Texture.mName		+= "(";
		Texture.mName		+= TFTexture.uri.c_str();
		Texture.mName		+= ")";
	}

	// Buffer Loading
	maSceneBuffer.resize( TLModels.accessors.size() );
	for(zUInt idxBuffer(0), cntBuffer(maSceneBuffer.size()); idxBuffer<cntBuffer; ++idxBuffer)
	{
		const auto& TFView	= TLModels.accessors[idxBuffer];
		const auto& TFBuffer= TLModels.bufferViews[TFView.bufferView];
		auto& Buffer		= maSceneBuffer[idxBuffer];
		const zU8* pRawData	= &TLModels.buffers[TFBuffer.buffer].data[TFBuffer.byteOffset+TFView.byteOffset];		
		size_t ElemSize		= tinygltf::GetComponentSizeInBytes(TFView.componentType);
		size_t ElemCount	= tinygltf::GetNumComponentsInType(TFView.type);
		size_t Stride		= ElemSize*ElemCount;
		if( TFBuffer.byteStride != 0 )
		{ assert(0); } //Copy Data contiguous

		Buffer.mrBuffer		= zenRes::zGfxBuffer::Create(pRawData, TFBuffer.byteLength, Stride, static_cast<zU32>(TFView.count));
		Buffer.mName		= TFView.name.c_str();
	}
	
	// Mesh Loading
	maSceneMesh.resize( TLModels.meshes.size());
	for(zUInt idxMesh(0), cntMesh(maSceneMesh.size()); idxMesh<cntMesh; ++idxMesh)
	{
		const auto& TFMesh	= TLModels.meshes[idxMesh];
		auto& Mesh			= maSceneMesh[idxMesh];
		Mesh.mName			= TFMesh.name.c_str();
		Mesh.marStrip.resize(TFMesh.primitives.size());
		for(zUInt idxStrip(0), cntStrip(Mesh.marStrip.size()); idxStrip<cntStrip; ++idxStrip)
		{
			const auto& TFStrip				= TFMesh.primitives[idxStrip];			
			auto& rStrip					= Mesh.marStrip[idxStrip];
			zU64 hBufferIndexKey			= (zU64(TFStrip.indices) << 32) | TFStrip.mode;
			zenRes::zGfxIndex& rBufferIndex	= mdSceneBufferIndex[hBufferIndexKey];
			if( rBufferIndex.IsValid() == false )
			{
				const auto& TFView		= TLModels.accessors[TFStrip.indices];
				const auto& TFBuffer	= TLModels.bufferViews[TFView.bufferView];
				const zenConst::ePrimitiveType TFtoZenPrimType[] = {
					zenConst::ePrimitiveType::kePrimType_PointList, // TINYGLTF_MODE_POINTS (0)
					zenConst::ePrimitiveType::kePrimType_LineList, // TINYGLTF_MODE_LINE (1)
					zenConst::ePrimitiveType::kePrimType_LineList, // TINYGLTF_MODE_LINE_LOOP (2)
					zenConst::ePrimitiveType::kePrimType_TriangleList, // TINYGLTF_MODE_TRIANGLES (4)
					zenConst::ePrimitiveType::kePrimType_TriangleStrip,// TINYGLTF_MODE_TRIANGLE_STRIP (5)
					zenConst::ePrimitiveType::kePrimType__Count // (Unsupported) TINYGLTF_MODE_TRIANGLE_FAN (6)
				};

				if( tinygltf::GetComponentSizeInBytes(TFView.componentType) == 2 )
				{
					zArrayDyn<zU16> aIndexData;
					const zU16* pRawData	= reinterpret_cast<const zU16*>(&TLModels.buffers[TFBuffer.buffer].data[TFBuffer.byteOffset+TFView.byteOffset]);					
					aIndexData.Copy(pRawData, TFView.count);
					rBufferIndex			= zenRes::zGfxIndex::Create(aIndexData, TFtoZenPrimType[TFStrip.mode]);
				}
				else if( tinygltf::GetComponentSizeInBytes(TFView.componentType) == 4 )
				{
					zArrayDyn<zU32> aIndexData;
					const zU32* pRawData	= reinterpret_cast<const zU32*>(&TLModels.buffers[TFBuffer.buffer].data[TFBuffer.byteOffset+TFView.byteOffset]);					
					aIndexData.Copy(pRawData, TFView.count);
					rBufferIndex			= zenRes::zGfxIndex::Create(aIndexData, TFtoZenPrimType[TFStrip.mode]);
				}
				else
				{
					zenAssert(0);
				}
			}
			//const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst, zU32 _uIndexCount, zU32 _uVertexFirst)
			rStrip = zenRes::zGfxMeshStrip::Create(rBufferIndex, mrShaderBind);
			const zU64 kNormal	= zU64('N') | zU64('O')<<8 | zU64('R')<<16 | zU64('M')<<24 | zU64('A')<<32 | zU64('L')<<40;
			const zU64 kPos		= zU64('P') | zU64('O')<<8 | zU64('S')<<16 | zU64('I')<<24 | zU64('T')<<32 | zU64('I')<<40;
			const zU64 kTan		= zU64('T') | zU64('A')<<8 | zU64('N')<<16 | zU64('G')<<24 | zU64('E')<<32 | zU64('N')<<40;
			const zU64 kUV		= zU64('T') | zU64('E')<<8 | zU64('X')<<16 | zU64('C')<<24 | zU64('O')<<32 | zU64('O')<<40;
			for(const auto& attribute : TFStrip.attributes)
			{
				// Normal - (3)	T:3 N:0	CT:5126
				// Position	(1) T:3 N:0 CT:5126
				// Tangeant	(4) T:4 N:0 CT:5126
				// Textcord	(2) T:2 N:0 CT:5126
				const char* Name	= &attribute.first[0];
				auto& rInputBuffer	= maSceneBuffer[attribute.second].mrBuffer;
				zU64 First6Chars	= zU64(Name[0])<<0 | zU64(Name[1])<<8 | zU64(Name[2])<<16 | zU64(Name[3])<<24 | zU64(Name[4])<<32 | zU64(Name[5])<<40;
				
				switch( First6Chars )
				{
				case kPos:		rStrip.SetResource(zHash32("VInputPOSITION"), rInputBuffer); break;
				case kNormal:	rStrip.SetResource(zHash32("VInputNORMAL"), rInputBuffer); break;
				case kTan:		rStrip.SetResource(zHash32("VInputTANGEANT"), rInputBuffer); break;
				case kUV:		rStrip.SetResource(zHash32("VInputTEXCOORD_0"), rInputBuffer); break;
				}
				//zHash32 Key = zHash32("VInputPOSITION");
				
			}
		}
		Mesh.mrMesh = zenRes::zGfxMesh::Create(Mesh.marStrip);
		
	}
	
	matWorld.SetIdentity();
	return true;
}

void SampleGLTLInstance::UpdateBackbuffers()
{
	zenPerf::zScopedEventCpu EmitEvent("UpdateBackbuffers");
	if( !mrBackbufferDepth.IsValid() || mrMainWindowGfx.PerformResize() )
	{	
		zVec2U16 vBackbufferDim		= mrMainWindowGfx.GetBackbuffer().GetDim();
		mrBackbufferDepth			= zenRes::zGfxTarget2D::Create(zenConst::keTexFormat_D32, vBackbufferDim ); 
		zenMath::MatrixProjectionLH( matProjection, 60, float(vBackbufferDim.y)/float(vBackbufferDim.x), 0.01f, 10000.f );
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

void SampleGLTLInstance::Destroy()
{
	Super::Destroy();
}

void SampleGLTLInstance::Update()
{	
	Super::Update();
	zenPerf::zScopedEventCpu EmitEventUpdate("SampleDirextX12Instance::Update");

	//---------------------------------------------------------------------
	// Render loop
	//---------------------------------------------------------------------
	{	
	
	
		mrMainWindowGfx.FrameBegin();
		UpdateBackbuffers();
		{			
			zenPerf::zScopedEventCpu EmitEventDraw("Draw");
			zenGfx::zCommandList rCxtRender	= zenGfx::zCommandList::Create("RenderLoop", mrRndPassFinal);
			
			float t				= static_cast<float>(zenSys::GetElapsedSec() / 3.0);	// Update our time animation
			zVec4F vClearColor	= zenMath::TriLerp( zVec4F(0.05f,0.05f,0.05f,1), zVec4F(0.1f,0.1f,0.20f,1), zVec4F(0.05f,0.05f,0.05f,1), zenMath::Fract(t) );
	
			zenMath::V4 vEye	= zenMath::simdXYZW( 0.0f, 0.0f,-1000, 0.0f );
			zenMath::V4 vAt		= zenMath::simdXYZW( 0.0f, 0.0f, 0.0f, 0.0f );
			zenMath::V4 vUp		= zenMath::simdXYZW( 0.0f, 1.0f, 0.0f, 0.0f );
			zenMath::MatrixLookAtLH(matView, vEye, vAt, vUp );
		
			zenGfx::zCommand::ClearColor(rCxtRender, mrMainWindowGfx.GetBackbuffer(), vClearColor);
			zenGfx::zCommand::ClearDepthStencil(rCxtRender, mrBackbufferDepth);
			for(zUInt i(0); i<maSceneMesh.size(); ++i )
			{
				maSceneMesh[i].mrMesh.SetValue( zHash32("World"),		matWorld );
				maSceneMesh[i].mrMesh.SetValue( zHash32("View"),		matView );
				maSceneMesh[i].mrMesh.SetValue( zHash32("Projection"),	matProjection );	
				zenGfx::zCommand::DrawMesh(rCxtRender, 0, maSceneMesh[i].mrMesh);
			}
			rCxtRender.Submit();		
		}		
		mrMainWindowGfx.FrameEnd();
	}

}
}
