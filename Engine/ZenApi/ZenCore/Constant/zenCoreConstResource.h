#pragma once
#ifndef __zenApi_Core_Const_Resource_h__
#define __zenApi_Core_Const_Resource_h__

namespace zen { 
	namespace zenConst 
	{
		enum eResSource
		{
			keResSource_Offline,	//!< Being exported in offline mode (converted to 'Loaded' on save)
			keResSource_Loaded,		//!< Loaded from a package
			keResSource_Runtime,	//!< Created at runtime, and using generated name
			keResSource__Count,
			keResSource__Invalid,
		};

		enum eResPlatform
		{
			//--- Renderer ---
			keResPlatform_DX11,

			//--- OS ---
			keResPlatform_Window,

			//--- ... ---
			keResPlatform__Count,
			keResPlatform__Invalid,
		};	

		enum eResType{
			//--- GFX ---
			keResType_GfxShaderPixel,
			keResType_GfxShaderVertex,
			keResType_GfxShaderParamDef,
			keResType_GfxShaderParam,
			keResType_GfxShaderBinding,
			keResType_GfxTarget2D,
			keResType_GfxView,
			keResType_GfxWindow,
			keResType_GfxIndex,
			keResType_GfxVertex,
			keResType_GfxBuffer,
			keResType_GfxTexture2D,
			keResType_GfxSampler,
			keResType_GfxBlend,
			keResType_GfxDepthStencil,
			keResType_GfxRaster,
			keResType_GfxMeshStrip,
			keResType_GfxMesh,
			keResType_GfxInputStream,
			keResType_GfxInputSignature,
			keResType_GfxRenderPass,
			//--- .... ---	
			keResType__Count,
			keResType__Invalid=keResType__Count,
		};

		enum eShaderElementType
		{
			keShaderElemType_Float,
			keShaderElemType_SInt,
			keShaderElemType_UInt,
			keShaderElemType_UByte,
			keShaderElemType_SByte,
			keShaderElemType__Count,
			keShaderElemType__Invalid=keShaderElemType__Count
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
		typedef zFlag<zenConst::eResType,zenConst::keResType__Count, zU64>		zFlagResType;
		typedef zFlag<zenConst::eResPlatform,zenConst::keResPlatform__Count>	zFlagResPlatform;
		typedef zFlag<zenConst::eResourceUse,zenConst::keResUse__Count>			zFlagResUse;
		typedef zFlag<zenConst::eTextureUse,zenConst::keTexCreate__Count>		zFlagResTexUse;		
	}

	namespace zenConst
	{
		const zFlagResType kFlagResShaders = zFlagResType(keResType_GfxShaderPixel, keResType_GfxShaderVertex);
	}



}  //namespace zen

#include ZENHeaderPlatform(zenCoreConstResource)
#include ZENHeaderRenderer(zenCoreConstResource)

#endif
