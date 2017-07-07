#pragma once

namespace zen { namespace zenRes {	
	//! @todo Clean: Expose shader compiling to runtime?
	class zGfxShaderVertex : public zcRes::GfxShaderVertexRef
	{
	zenClassDeclare(zGfxShaderVertex, zcRes::GfxShaderVertexRef);
	public:
		using Super::Super;
		using Super::operator=;

		static zGfxShaderVertex Create(const char* _zFilename, const char* _zEntryname );
		static zGfxShaderVertex Create(const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

	class zGfxShaderPixel : public zcRes::GfxShaderPixelRef
	{
	zenClassDeclare(zGfxShaderPixel, zcRes::GfxShaderPixelRef);
	public:
		using Super::Super;
		using Super::operator=;

		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname );
		static zGfxShaderPixel Create( const char* _zFilename, const char* _zEntryname, const zArrayBase<zShaderDefine>& _aDefines);
	};

	class zGfxCBuffer : public zcRes::GfxCBufferRef
	{
	zenClassDeclare(zGfxCBuffer, zcRes::GfxCBufferRef);
	public:
		using Super::Super;
		using Super::operator=;
		
		//void					SetValue(const zcExp::ParameterBase& _Value);	
		void					SetValue(const zenRes::zShaderParameter& _Value);
		void					SetValue(zHash32 _hParamName, float _fValue);
		void					SetValue(zHash32 _hParamName, zU32 _uValue);
		void					SetValue(zHash32 _hParamName, const zVec2F& _vValue);
		void					SetValue(zHash32 _hParamName, const zVec3F& _vValue);
		void					SetValue(zHash32 _hParamName, const zVec4F& _vValue);
		void					SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue);

		static zGfxCBuffer		Create(const zGfxShaderPixel& _rShader, zU8 _uCBufferSlot);
		static zGfxCBuffer		Create(const zGfxShaderPixel& _rShader, zHash32 _uCBufferName);
		static zGfxCBuffer		Create(const zGfxShaderVertex& _rShader, zU8 _uCBufferSlot);
		static zGfxCBuffer		Create(const zGfxShaderVertex& _rShader, zHash32 _uCBufferName);
	};
}} // namespace zen, zenRes
