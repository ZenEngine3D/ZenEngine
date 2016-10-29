#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_h__
#define __zCore_Res_Gfx_GfxShaderParam_h__

#include ZENHeaderRenderer( zcResGfxShaderParam )

namespace zcRes
{
	class GfxCBufferDefinition : protected GfxCBufferDefinitionHAL
	{
	zenClassDeclare(GfxCBufferDefinition, GfxCBufferDefinitionHAL)
	public:
	};

	class GfxCBuffer : protected GfxCBufferHAL
	{
	zenClassDeclare(GfxCBuffer, GfxCBufferHAL)
	public:
		const zcRes::GfxCBufferDefinitionRef& GetParent();
		using Super::SetValue;
	};	
}

#endif
