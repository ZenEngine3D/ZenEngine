#pragma once
#ifndef __zenCore_Const_Resource_h__
#define __zenCore_Const_Resource_h__

namespace zen { 
	namespace zenConst 
	{
		enum eResSource
		{
			keResSource_Offline,		//!< Being exported in offline mode (converted to 'Loaded' on save)
			keResSource_Loaded,		//!< Loaded from a package
			keResSource_Runtime,		//!< Created at runtime, and using generated name
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
			keResType_GfxRenderTarget,
			keResType_GfxView,
			keResType_GfxWindow,
			keResType_GfxIndex,
			keResType_GfxVertex,			
			keResType_GfxTexture2D,
			keResType_GfxSampler,
			keResType_GfxBlend,
			keResType_GfxDepthStencil,
			keResType_GfxRasterizer,
			keResType_GfxMeshStrip,
			keResType_GfxMesh,
			keResType_GfxInputStream,
			keResType_GfxInputSignature,
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

		enum eBlend
		{
			keBlend_Zero,
			keBlend_One,
			keBlend_SrcColor,
			keBlend_InvSrcColor,
			keBlend_SrcAlpha,
			keBlend_InvSrcAlpha,
			keBlend_DestAlpha,
			keBlend_InvDestAlpha,
			keBlend_DestColor,
			keBlend_InvDestColor,
			keBlend_SrcAlphaSat,
			keBlend_BlendFactor,
			keBlend_InvBlendFactor,
			keBlend_Src1Color,
			keBlend_InvSrc1Color,
			keBlend_Src1Alpha,
			keBlend_InvSrc1Alpha
		};

		enum eBlendOp
		{
			keBlendOp_Add,
			keBlendOp_Substract,
			keBlendOp_RevSubstract,
			keBlendOp_Min,
			keBlendOp_Max
		};

		enum eColorWrite
		{
			keColorWrite_None	= 0x00,
			keColorWrite_Red	= 0x01,
			keColorWrite_Green	= 0x02,
			keColorWrite_Blue	= 0x04,
			keColorWrite_Alpha	= 0x08,
			keColorWrite_RGB	= ( ( keColorWrite_Red | keColorWrite_Green )  | keColorWrite_Blue ),
			keColorWrite_RGBA	= ( ( ( keColorWrite_Red | keColorWrite_Green )  | keColorWrite_Blue )  | keColorWrite_Alpha ) 
		};

		enum eComparisonFunc
		{
			keComparisonFunc_Never,
			keComparisonFunc_Less,
			keComparisonFunc_Equal,
			keComparisonFunc_LessEqual,
			keComparisonFunc_Greater,
			keComparisonFunc_NotEqual,
			keComparisonFunc_GreaterEqual,
			keComparisonFunc_Always
		};

		enum eStencilOp
		{
			keStencilOp_Keep,
			keStencilOp_Zero,
			keStencilOp_Replace,
			keStencilOp_IncrSat,
			keStencilOp_DecrSat,
			keStencilOp_Invert,
			keStencilOp_Incr,
			keStencilOp_Decr
		};

		enum eCullMode
		{
			keCullMode_None,
			keCullMode_Front,
			keCullMode_Back
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

		enum eResourceUse
		{
			keResUse_Static,		// Never updated once created
			keResUse_Dynamic,		// Can be updated once per frame
			keResUse_Volatile,		// Can be updated multiple time per frame
			keResUse__Count,
			keResUse__Invalid
		};
	}

	namespace zenType
	{
		typedef zFlag<zenConst::eResType,zenConst::keResType__Count, zU64>	zFlagResType;
		typedef zFlag<zenConst::eResType,zenConst::keResPlatform__Count>	zFlagResPlatform;
		typedef zFlag<zenConst::eResourceUse,zenConst::keResUse__Count>		zFlagResUse;
		typedef zFlag<zenConst::eTextureUse,zenConst::keTexCreate__Count>	zFlagResTexUse;		
	}

	namespace zenConst
	{
		const zFlagResType kFlagResShaders = zFlagResType(keResType_GfxShaderPixel, keResType_GfxShaderVertex);
	}



}  //namespace zen

#include ZENHeaderPlatform(zenConstResource)
#include ZENHeaderRenderer(zenConstResource)

#endif
