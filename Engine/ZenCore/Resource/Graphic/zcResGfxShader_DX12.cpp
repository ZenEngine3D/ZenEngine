#include "zcCore.h"

namespace zcRes
{
//=================================================================================================
GfxShaderVertex_DX12::~GfxShaderVertex_DX12()
{
}

bool GfxShaderVertex_DX12::Initialize()
{		
	mDXShaderCode.pShaderBytecode	= maCompiledShader.First();
	mDXShaderCode.BytecodeLength	= maCompiledShader.SizeMem();
	return true;
}

//=================================================================================================
GfxShaderPixel_DX12::~GfxShaderPixel_DX12()
{	
}

bool GfxShaderPixel_DX12::Initialize()
{		
	mDXShaderCode.pShaderBytecode	= maCompiledShader.First();
	mDXShaderCode.BytecodeLength	= maCompiledShader.SizeMem();
	return true;
}

}
