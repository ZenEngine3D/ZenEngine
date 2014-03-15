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

		TextureBinding(zHash32 _hTextureName, zResID _TextureID, zResID _SamplerID)
		: mhTextureName(_hTextureName)
		, mTextureID(_TextureID)	
		, mSamplerID(_SamplerID)
		{}

	 	zHash32		mhTextureName;
		zResID	mTextureID;
		zResID	mSamplerID;
	};
	*/
	
	class SerialMeshStrip_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialMeshStrip_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zResID							mVertexBufferID;
			zResID							mIndexBufferID;
			zResID							mShaderBindingID;			
			zArrayStatic<zenRes::zShaderTexture>	maTexture;				//!< List of textures assigned to shader
			zArrayStatic<zResID>				maShaderParamID;		//!< List of Shader Param Instance (1 per ParamDef entry)
			zU32									muIndexFirst;
			zU32									muIndexCount;
		};
	};
	
	zResID CreateGfxMeshStrip(zResID _VertexBufferID, zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zArrayBase<zResID>& _aShaderParamID=zArrayStatic<zResID>(), const zArrayBase<zenRes::zShaderTexture>& _aTexture=zArrayStatic<zenRes::zShaderTexture>());
}

#include "EExpMeshStripDX11.h"

#endif