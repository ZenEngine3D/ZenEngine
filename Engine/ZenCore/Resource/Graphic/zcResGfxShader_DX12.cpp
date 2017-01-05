#include "zcCore.h"

namespace zcRes
{
//=================================================================================================
GfxShaderVertexHAL_DX12::~GfxShaderVertexHAL_DX12()
{
}

bool GfxShaderVertexHAL_DX12::Initialize()
{		
	mDXShaderCode.pShaderBytecode	= maCompiledShader.First();
	mDXShaderCode.BytecodeLength	= maCompiledShader.SizeMem();
	return true;
}

//=================================================================================================
GfxShaderPixelHAL_DX12::~GfxShaderPixelHAL_DX12()
{	
}

bool GfxShaderPixelHAL_DX12::Initialize()
{		
	mDXShaderCode.pShaderBytecode	= maCompiledShader.First();
	mDXShaderCode.BytecodeLength	= maCompiledShader.SizeMem();
	return true;
}

}
