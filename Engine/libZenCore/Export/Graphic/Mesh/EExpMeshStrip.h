#pragma once
#ifndef __LibExport_Graphics_MeshStrip_Base_h__
#define __LibExport_Graphics_MeshStrip_Base_h__

namespace EExp
{
	/*
	struct TextureBinding
	{
		TextureBinding()
		: mhTextureName("")
		, mTextureID()
		, mSamplerID()
		{}

		TextureBinding(zenHash32 _hTextureName, zenResID _TextureID, zenResID _SamplerID)
		: mhTextureName(_hTextureName)
		, mTextureID(_TextureID)	
		, mSamplerID(_SamplerID)
		{}

	 	zenHash32		mhTextureName;
		zenResID	mTextureID;
		zenResID	mSamplerID;
	};
	*/
	
	class SerialMeshStrip_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialMeshStrip_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zenResID							mVertexBufferID;
			zenResID							mIndexBufferID;
			zenResID							mShaderBindingID;			
			zenArrayStatic<awres::awShaderTexture>	maTexture;				//!< List of textures assigned to shader
			zenArrayStatic<zenResID>				maShaderParamID;		//!< List of Shader Param Instance (1 per ParamDef entry)
			zenU32									muIndexFirst;
			zenU32									muIndexCount;
		};
	};
	
	zenResID CreateGfxMeshStrip(zenResID _VertexBufferID, zenResID _IndexBufferID, zenResID _ShaderBindingID, zenU32 _uIndexFirst=0, zenU32 _uIndexCount=0xFFFFFFFF, const zenArrayBase<zenResID>& _aShaderParamID=zenArrayStatic<zenResID>(), const zenArrayBase<awres::awShaderTexture>& _aTexture=zenArrayStatic<awres::awShaderTexture>());
}

#include "EExpMeshStripDX11.h"

#endif