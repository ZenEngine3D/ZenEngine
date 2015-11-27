#pragma once
#if !defined(__zCore_Exp_Gfx_GfxMeshStrip_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxMeshStrip_DX11_h__

namespace zcExp
{	
	class ResDataGfxMeshStripDX11 : public ResourceData
	{
	ZENClassDeclare(ResDataGfxMeshStripDX11, ResourceData)
	public:
		virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zResID								mIndexBufferID;
		zResID								mStreamBindingID;
		zResID								mShaderBindingID;
		zArrayStatic<zResID>				maShaderParamID;		//!< Array of all ShaderParam used by all Shaders stage
		zArrayStatic<zArrayStatic<zResID>>	maTextureID;			//!< Per Shader stage texture input for each slot
		zArrayStatic<zArrayStatic<zResID>>	maSamplerID;			//!< Per Shader stage sampler state for each slot
		zU32								muVertexFirst;
		zU32								muIndexFirst;
		zU32								muIndexCount;
	};	
	
}

#endif