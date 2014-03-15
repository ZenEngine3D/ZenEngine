#pragma once
#ifndef __LibResource_Graphics_GfxShaderParam_DX11_h__
#define __LibResource_Graphics_GfxShaderParam_DX11_h__

namespace ERes
{
class GfxShaderParamDef_DX11 : public Resource
{
ZENResourceDeclare(GfxShaderParamDef_DX11, EExp::SerialGfxShaderParamDef_DX11, zenConst::keResType_GfxShaderParamDef)
public:
	virtual bool				ResourceInit();
};

class GfxShaderParam_DX11 : public Resource
{
	struct InstanceInfo : public EExp::SerialGfxShaderParam_DX11
	{
		GfxShaderParamDefRef		mpParentShaderParamDef;
		ID3D11Buffer*				mpBufferBinding;
	};
ZENResourceDeclare(GfxShaderParam_DX11, InstanceInfo, zenConst::keResType_GfxShaderParam)
public:
	virtual						~GfxShaderParam_DX11();
	virtual bool				ResourceInit();

//-----------------------------------------------------------------------------
// Common to all ShaderParam
public:	
	void						Bind(EExp::eShaderStage _eShaderStage)const;	
	void						SetValue(const zenRes::zShaderParameter& _Value);	
	void						SetValue(const EExp::ParameterBase& _Value);	
	void						SetValue(const zHash32& _hParamName, const float& _fValue);
	void						SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
	void						SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
	void						SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
	void						SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
//-----------------------------------------------------------------------------

protected:
	mutable bool				mbUpdated;
};
	
}

#endif