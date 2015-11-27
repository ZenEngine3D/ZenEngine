#pragma once
#ifndef __zCore_Res_Gfx_GfxShaderParam_DX11_h__
#define __zCore_Res_Gfx_GfxShaderParam_DX11_h__

namespace zcRes
{
	class GfxShaderParamDefProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxShaderParamDefProxy_DX11, zRefCounted)
	public:
												GfxShaderParamDefProxy_DX11();
		virtual									~GfxShaderParamDefProxy_DX11();
		bool									Initialize(class GfxShaderParamDef& _Owner);
				
		zcExp::eShaderParamFreq					meFrequence;		
		zcExp::ShaderParamItemInfoMap			mdParameters;
		zArrayStatic<zU8>						maParameterDefaults;
		ZENDbgCode(class GfxShaderParamDef*		mpOwner);
	};

	class GfxShaderParamProxy_DX11 : public zRefCounted
	{
	ZENClassDeclare(GfxShaderParamProxy_DX11, zRefCounted)
	public:
												GfxShaderParamProxy_DX11();
		virtual									~GfxShaderParamProxy_DX11();
		bool									Initialize(class GfxShaderParam& _Owner);
		
		void									Bind(zenConst::eShaderStage _eShaderStage)const;	
		void									SetValue(const zenRes::zShaderParameter& _Value);	
		void									SetValue(const zcExp::ParameterBase& _Value);	
		void									SetValue(const zHash32& _hParamName, const float& _fValue);
		void									SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void									SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void									SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void									SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		
		GfxShaderParamDefRef					mrProxShaderParamDef;
		ID3D11Buffer*							mpBufferBinding;
		zArrayStatic<zU8>						maParameterValues;	//!< Bytes data for all buffer values (see ShaderParamDef for each parameter start offset in array)
		mutable bool							mbUpdated;
		ZENDbgCode(class GfxShaderParam*		mpOwner);
	};	
}

#endif