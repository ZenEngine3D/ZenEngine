#pragma once
#ifndef __zenApi_Core_Const_Graphic_h__
#define __zenApi_Core_Const_Graphic_h__

namespace zen { namespace zenConst 
{
	enum eShaderStage
	{
		keShaderStage_Vertex, 
		keShaderStage_Pixel,
		keShaderStage__Count,
		keShaderStage__Invalid
	};

	enum eShaderResource
	{ 
		keShaderRes_Texture, 
		keShaderRes_Sampler, 
		keShaderRes_CBuffer,	//! @todo clean
		keShaderRes_Buffer, 
		keShaderRes_BufferRW,	//! @todo clean
		keShaderRes_Unknown, 
		keShaderRes__Count, 
		keShaderRes__Invalid=keShaderRes__Count 
	};

	enum eColorChannel
	{
		keColor_R,
		keColor_G,
		keColor_B,
		keColor_A,
		keColor__Count,
		keColor__Invalid
	};
}

namespace zenType 
{
	typedef zFlag<zenConst::eColorChannel, zenConst::keColor__Count> zColorMask;	
}

namespace zenConst 
{
	const zColorMask kColorMaskRGBA(keColor_R, keColor_G, keColor_B, keColor_A);
	const zColorMask kColorMaskRGB(keColor_R, keColor_G, keColor_B);
	const zColorMask kColorMaskRG(keColor_R, keColor_G);
	const zColorMask kColorMaskR(keColor_R);
	const zColorMask kColorMaskA(keColor_A);
}}  //namespace zen, Const 

#endif