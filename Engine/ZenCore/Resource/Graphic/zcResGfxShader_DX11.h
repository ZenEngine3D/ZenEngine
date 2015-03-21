#pragma once
#ifndef __zCore_Res_Gfx_Shaders_DX11_h__
#define __zCore_Res_Gfx_Shaders_DX11_h__

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

namespace zcRes
{
	typedef zMap<zcExp::SerialShader_DX11::BindInfo>::Key32	MapBindInfo;

	//=============================================================================================
	//! @class	Empty vertex shader used to match vertex shader signature.
	//!			This allow every vertex shader sharing the same input signature, to use the same
	//!			ShaderCode when creating a InputLayout, and to not create extra one if they have
	//!			the same configuration. 
	//!			Also remove the need to keep the vertex shader code loaded, after creating 
	//!			it's shader, since the signature contain the relevant part.
	//=============================================================================================
	class GfxInputSignature_DX11 : public Resource
	{
	ZENResourceDeclare(GfxInputSignature_DX11, zcExp::SerialGfxInputSignature_DX11, zenConst::keResType_GfxInputSignature )
	};

	//=============================================================================================
	//! @class	Contains Common Shader infos
	//=============================================================================================
	class GfxShader_DX11 : public Resource
	{
	protected:
		struct InstanceInfoBase
		{			
			zArrayStatic<GfxShaderParamDefRef>	maGfxShaderParamDefRef;	//! <List of Constant buffer (per frequency) associated with this shader
			MapBindInfo							mdTextureSamplerSlot;	//! <Dictionary of texture/sampler name used by shader, with associated texture slot		
		};
	ZENClassDeclare(GfxShader_DX11, Resource)
	public:
											GfxShader_DX11(zcExp::SerialShader_DX11& _SerialItem, InstanceInfoBase& _CommonInstance);
		virtual bool						ResourceInit();
		ZENInline const InstanceInfoBase&		GetInstanceBase(){return mBaseInstance;}
		GfxShaderParamDefRef				GetShaderParamDef(zcExp::eShaderParamFreq _eShaderParam)const;
		zcExp::eShaderStage					GetShaderStage()const{return mBaseSerialItem.meShaderStage;}		
		const MapBindInfo&					GetTextureSlotInfo()const{return mBaseInstance.mdTextureSamplerSlot;}
		
	protected:
		InstanceInfoBase&					mBaseInstance;
		zcExp::SerialShader_DX11&			mBaseSerialItem;
	};

	//=============================================================================================
	//! @class	Contains Vertex Shader 
	//=============================================================================================
	class GfxShaderVertex_DX11 : public GfxShader_DX11
	{
		struct InstanceInfo : public zcExp::SerialGfxShaderVertex_DX11, public GfxShader_DX11::InstanceInfoBase
		{
			ID3D11VertexShader*				mpVertexShader;
			GfxInputSignatureRef			mGfxInputSignatureRef;
		};
	ZENResourceDeclareParent(GfxShaderVertex_DX11, GfxShader_DX11, InstanceInfo, zenConst::keResType_GfxShaderVertex)
	public:
		virtual								~GfxShaderVertex_DX11();
		virtual bool						ResourceInit();
	};

	//=============================================================================================
	//! @class	Contains Pixel Shader
	//=============================================================================================
	class GfxShaderPixel_DX11 : public GfxShader_DX11
	{
		struct InstanceInfo : public zcExp::SerialGfxShaderPixel_DX11, public GfxShader_DX11::InstanceInfoBase
		{
			ID3D11PixelShader*				mpPixelShader;
		};
	ZENResourceDeclareParent(GfxShaderPixel_DX11, GfxShader_DX11, InstanceInfo, zenConst::keResType_GfxShaderPixel)
	public:
		virtual								~GfxShaderPixel_DX11();
		virtual bool						ResourceInit();		
	};

}

#endif