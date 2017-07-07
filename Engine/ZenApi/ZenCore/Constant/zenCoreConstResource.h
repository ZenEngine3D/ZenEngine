#pragma once

namespace zen { 
	namespace zenConst 
	{
		enum eResSource : zU8
		{
			keResSource_Offline,	//!< Being exported in offline mode (converted to 'Loaded' on save)
			keResSource_Loaded,		//!< Loaded from a package
			keResSource_Runtime,	//!< Created at runtime, and using generated name
			keResSource__Count,
			keResSource__Invalid=keResSource__Count,
		};

		enum ePlatform : zU8
		{
			//--- Graphics Api ---
			kePlatform__GfxFirst,
			kePlatform_DX11=kePlatform__GfxFirst,
			kePlatform_DX12,
			kePlatform__GfxLast=kePlatform_DX12,

			//--- OS ---
			kePlatform__OSFirst,
			kePlatform_Window=kePlatform__OSFirst,
			kePlatform__OSLast=kePlatform_Window,
			//--- ... ---
			kePlatform__Count,
			kePlatform__Invalid=kePlatform__Count,
		};	

		enum eResType : zU8 {
			//--- GFX ---
			keResType_GfxShaderPixel,
			keResType_GfxShaderVertex,
			keResType_GfxCBufferDefinition,
			keResType_GfxCBuffer,
			keResType_GfxShaderBinding,
			keResType_GfxTarget2D,
			keResType_GfxView,
			keResType_GfxWindow,
			keResType_GfxIndex,
			keResType_GfxBuffer,
			keResType_GfxTexture2D,
			keResType_GfxStateSampler,
			keResType_GfxStateBlend,
			keResType_GfxStateDepthStencil,
			keResType_GfxStateRaster,
			keResType_GfxMeshStrip,
			keResType_GfxMesh,
			keResType_GfxRenderPass,
			//--- .... ---	
			keResType__Count,
			keResType__Invalid=keResType__Count,
		};
		
		enum eShaderElementType
		{
			keShaderElemType_Float,
			keShaderElemType_Int,
			keShaderElemType_UInt,
			keShaderElemType_Byte,
			keShaderElemType_Char,
			keShaderElemType__Count,
			keShaderElemType__Invalid=keShaderElemType__Count
		};

		enum eShaderCBufferParamType
		{
			keShaderCBuffType_Float,	keShaderCBuffType_Float2, keShaderCBuffType_Float3, keShaderCBuffType_Float4,
			keShaderCBuffType_Int,		keShaderCBuffType_Int2, keShaderCBuffType_Int3, keShaderCBuffType_Int4,
			keShaderCBuffType_UInt,		keShaderCBuffType_UInt2, keShaderCBuffType_UInt3, keShaderCBuffType_UInt4,
			keShaderCBuffType_Byte,		keShaderCBuffType_Byte2, keShaderCBuffType_Byte3, keShaderCBuffType_Byte4,
			keShaderCBuffType_Char,		keShaderCBuffType_Char2, keShaderCBuffType_Char3, keShaderCBuffType_Char4,
			keShaderCBuffType_Bool,		keShaderCBuffType_Bool2, keShaderCBuffType_Bool3, keShaderCBuffType_Bool4,
			keShaderCBuffType_Struct, 
			keShaderCBuffType__Count,
			keShaderCBuffType__Invalid=keShaderCBuffType__Count
		};

		enum eShaderSemantic
		{
			keShaderSemantic_Position,
			keShaderSemantic_UV,
			keShaderSemantic_Color,
			keShaderSemantic_Normal,
			keShaderSemantic_Binormal,
			keShaderSemantic_Tangent,
			keShaderSemantic_BlendIndex,
			keShaderSemantic_BlendWeight,
			keShaderSemantic__Count,
			keShaderSemantic__Invalid
		};

		enum eTextureFormat
		{
			keTexFormat_R8,		
			keTexFormat_RGB8,
			keTexFormat_RGBA8,			
			keTexFormat_BC1,
			keTexFormat_BC2,
			keTexFormat_BC3,
			keTexFormat_BC5,
			keTexFormat_BC7,
			keTexFormat_RGBA32f,
			keTexFormat__DepthFirst,
			keTexFormat_D24S8		= keTexFormat__DepthFirst,
			keTexFormat_D32,		
			keTexFormat__DepthLast	= keTexFormat_D32,
			keTexFormat__Count,		
			keTexFormat__Invalid=keTexFormat__Count
		};

		//! @todo clean remove all those config as much as possible, move it to api res
		enum eTextureFiltering
		{
			keTexFilter_Point,		
			keTexFilter_Bilinear,
			keTexFilter_Trilinear,
			keTexFilter_Anisotropic,
			keTexFilter__Count,
			keTexFilter__Invalid=keTexFilter__Count
		};

		enum eTextureWrap
		{
			keTexWrap_Clamp,
			keTexWrap_Repeat,
			keTexWrap_Mirror,
			keTexWrap_Border,
			keTexWrap__Count,
			keTexWrap__Invalid=keTexWrap__Count
		};

		enum eTextureUse
		{
			keTexCreate_RenderTarget,	// Used for rendertarget
			keTexCreate__Count,
			keTexCreate__Invalid=keTexCreate__Count
		};

		enum ePrimitiveType
		{
			kePrimType_PointList,
			kePrimType_LineList,
			kePrimType_LineStrip,
			kePrimType_TriangleList,
			kePrimType_TriangleStrip,
			kePrimType__Count
		};

		//! @todo optim proper buffer access config
		enum eResourceUse
		{
			keResUse_Static,			// Never updated once created
			keResUse_Dynamic,			// Can be updated once per frame
			keResUse_DynamicDiscard,	// Can be updated once per frame, and don't care about previous frame content
			keResUse_Volatile,			// Can be updated multiple time per frame
			keResUse__Count,
			keResUse__Invalid
		};
	}

	namespace zenType
	{
		typedef zFlag<eResType,		keResType__Count, zU64>	zFlagResType;
		typedef zFlag<ePlatform,	kePlatform__Count>		zFlagResPlatform;
		typedef zFlag<eResourceUse,	keResUse__Count>		zFlagResUse;
		typedef zFlag<eTextureUse,	keTexCreate__Count>		zFlagResTexUse;		
	}

	namespace zenConst
	{
		const zFlagResType kFlagResShaders = zFlagResType(keResType_GfxShaderPixel, keResType_GfxShaderVertex);
	}



}  //namespace zen

#include zenHeaderPlatform(zenCoreConstResource)
#include zenHeaderRenderer(zenCoreConstResource)
