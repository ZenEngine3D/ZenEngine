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

		TextureBinding(awHash32 _hTextureName, awResourceID _TextureID, awResourceID _SamplerID)
		: mhTextureName(_hTextureName)
		, mTextureID(_TextureID)	
		, mSamplerID(_SamplerID)
		{}

	 	awHash32		mhTextureName;
		awResourceID	mTextureID;
		awResourceID	mSamplerID;
	};
	*/
	
	class SerialMeshStrip_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialMeshStrip_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			awResourceID							mVertexBufferID;
			awResourceID							mIndexBufferID;
			awResourceID							mShaderBindingID;			
			awArrayStatic<awres::awShaderTexture>	maTexture;				//!< List of textures assigned to shader
			awArrayStatic<awResourceID>				maShaderParamID;		//!< List of Shader Param Instance (1 per ParamDef entry)
			awU32									muIndexFirst;
			awU32									muIndexCount;
		};
	};
	
	awResourceID CreateGfxMeshStrip(awResourceID _VertexBufferID, awResourceID _IndexBufferID, awResourceID _ShaderBindingID, awU32 _uIndexFirst=0, awU32 _uIndexCount=0xFFFFFFFF, const awArrayBase<awResourceID>& _aShaderParamID=awArrayStatic<awResourceID>(), const awArrayBase<awres::awShaderTexture>& _aTexture=awArrayStatic<awres::awShaderTexture>());
}

#include "EExpMeshStripDX11.h"

#endif