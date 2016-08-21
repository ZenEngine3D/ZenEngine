#pragma once
#ifndef __zenApi_Core_ResGfxShader_h__
#define __zenApi_Core_ResGfxShader_h__

namespace zen { namespace zenRes {	
	
	class zGfxShaderParamDef : public zcRes::GfxShaderParamDefRef
	{
	ZENClassDeclare(zGfxShaderParamDef, zcRes::GfxShaderParamDefRef);
	public:
		using Super::Super;
		using Super::operator=;
	};

	class zGfxShaderParam : public zcRes::GfxShaderParamRef
	{
	ZENClassDeclare(zGfxShaderParam, zcRes::GfxShaderParamRef);
	public:
		using Super::Super;
		using Super::operator=;
	};
	
	//! @todo Clean: Expose shader compiling to runtime?
	class zGfxShaderVertex : public zcRes::GfxShaderVertexRef
	{
	ZENClassDeclare(zGfxShaderVertex, zcRes::GfxShaderVertexRef);
	public:
		using Super::Super;
		using Super::operator=;

		static zGfxShaderVertex Create(const char* _zFilename, const char* _zEntryname );
		static zGfxShaderVertex Create(const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

	class zGfxShaderPixel : public zcRes::GfxShaderPixelRef
	{
	ZENClassDeclare(zGfxShaderPixel, zcRes::GfxShaderPixelRef);
	public:
		using Super::Super;
		using Super::operator=;

		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};
}} // namespace zen, zenRes

#endif

