#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_h__
#define __zCore_Res_Gfx_GfxShaderParam_h__

#include zenHeaderRenderer( zcResGfxShaderParam )

namespace zcRes
{
	class GfxCBufferDefinition : protected GfxCBufferDefinition_HAL
	{
	zenClassDeclare(GfxCBufferDefinition, GfxCBufferDefinition_HAL)
	public:
	};

	class GfxCBuffer : protected GfxCBuffer_HAL
	{
	zenClassDeclare(GfxCBuffer, GfxCBuffer_HAL)
	public:
		const zcRes::GfxCBufferDefinitionRef& GetParent();
		using Super::SetValue;
	};	
}

#endif
