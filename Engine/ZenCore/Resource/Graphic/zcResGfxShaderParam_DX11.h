#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_DX11_h__
#define __zCore_Res_Gfx_GfxShaderParam_DX11_h__

namespace zcRes
{
	class GfxShaderParamDefHAL_DX11 : public zcExp::ExportGfxShaderParamDefDX11
	{
	zenClassDeclare(GfxShaderParamDefHAL_DX11, zcExp::ExportGfxShaderParamDefDX11)
	public:
		bool												Initialize();				
		typedef zcExp::ExporterGfxShaderParamDefDX11_DX11	RuntimeExporter;
	};
	class GfxShaderParamDefHAL : public GfxShaderParamDefHAL_DX11{};
	
	class GfxShaderParamHAL_DX11 : public zcExp::ExportGfxShaderParamDX11
	{
	zenClassDeclare(GfxShaderParamHAL_DX11, zcExp::ExportGfxShaderParamDX11)
	public:
		virtual												~GfxShaderParamHAL_DX11();
		bool												Initialize();
		
		void												Bind(zenConst::eShaderStage _eShaderStage)const;	
		void												SetValue(const zenRes::zShaderParameter& _Value);	
		void												SetValue(const zcExp::ParameterBase& _Value);	
		void												SetValue(const zHash32& _hParamName, const float& _fValue);
		void												SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void												SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		
		zcRes::GfxShaderParamDefRef							mrShaderParamDef;
		ID3D11Buffer*										mpBufferBinding			= nullptr;
		mutable bool										mbUpdated				= false;
		
		typedef zcExp::ExporterGfxShaderParamDX11_DX11		RuntimeExporter;
	};
	class GfxShaderParamHAL : public GfxShaderParamHAL_DX11{};
}

#endif