#pragma once
#ifndef __LibResource_Graphics_Binding_DX11_h__
#define __LibResource_Graphics_Binding_DX11_h__

namespace ERes
{
	//=============================================================================================
	//! @class	Bind together all shader stage, Shader parameters, texture used and sampler
	//=============================================================================================
	class GfxShaderBinding_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxShaderBinding_DX11
		{
			zArrayStatic<GfxShaderRef>		maGfxShaderRef;			//!< Shader used for each shader stage			
			zMap<zU32>::Key32			mdCBuferPerParam;		//!< Bitmask of each ShaderParam containing this parameter name
			zMap<TextureSlot>::Key32		mdStageSlotPerTexture;	//!< Texture Index of each Shader stage using this parameter name		
		};

	AWResourceDeclare(GfxShaderBinding_DX11, InstanceInfo, zenConst::keResType_GfxShaderBinding)	
	public:												
		virtual bool	ResourceInit();
	};

	//=============================================================================================
	//! @class	Binding between a vertex shader in a VertexBuffer.
	//!			Needed for proper mapping of vertex stream into Shader.	
	//=============================================================================================
	class GfxInputStream_DX11 : public Resource
	{	
		struct InstanceInfo : public EExp::SerialGfxInputStream_DX11
		{
			ERes::GfxVertexRef			mrVertex;
			ERes::GfxInputSignatureRef	mrSignature;
			ID3D11InputLayout*			mpInputLayout;	//!< Contain DX object for vertex input remaping
		};

	AWResourceDeclare(GfxInputStream_DX11, InstanceInfo, zenConst::keResType_GfxInputStream)
	public:
		virtual			~GfxInputStream_DX11();
		virtual bool	ResourceInit();
	};

	//=============================================================================================
	//! @class	Binding geometry to shader
	//=============================================================================================
	class GfxMeshStrip_DX11 : public Resource
	{	
		struct InstanceInfo : public EExp::SerialGfxMeshStrip_DX11
		{
			GfxIndexRef										mrIndexBuffer;
			GfxInputStreamRef								mrInputStream;			//!< Reference to input streams binding
			GfxShaderBindingRef								mrShaderBinding;		//!< Reference to shader binding infos
			zArrayStatic<GfxShaderParamRef>				maShaderParamRef;		//!< Array of all ShaderParam used by all Shaders stage
			zArrayStatic<zArrayStatic<GfxTexture2DRef>>	maTextureRef;			//!< Per Shader stage texture input (Array sub Index = Slot)
			zArrayStatic<zArrayStatic<GfxSamplerRef>>		maGfxSamplerRef;		//!< Per Shader stage texture input (Array sub Index = Slot)			
		};	
	AWResourceDeclare(GfxMeshStrip_DX11, InstanceInfo, zenConst::keResType_GfxMeshStrip)	
	public:
		virtual bool	ResourceInit();

		//------------------------------------------------------------------------------------------------
		// Common to all MeshStrip
		//------------------------------------------------------------------------------------------------
		void			SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);	
		void			SetValue(const zenRes::zShaderParameter& _Value);
		void			SetValue(const zHash32& _hParamName, const float& _fValue);
		void			SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void			SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture );
		void			SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler );
		void			SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler );
		//------------------------------------------------------------------------------------------------
	};

	//=============================================================================================
	//! @class	Bind together all strip of a Mesh
	//=============================================================================================
	class GfxMesh_DX11 : public Resource
	{
		struct InstanceInfo : public EExp::SerialGfxMesh_DX11
		{
			zArrayStatic<GfxMeshStripRef>			maGfxMeshStripRef;			//!< Shader used for each shader stage			
		};
	AWResourceDeclare(GfxMesh_DX11, InstanceInfo, zenConst::keResType_GfxMesh)
	public:												
		virtual bool	ResourceInit();

		//------------------------------------------------------------------------------------------------
		// Common to all Mesh
		//------------------------------------------------------------------------------------------------
		void			SetValue(const zArrayBase<const zenRes::zShaderParameter*>& _aValues);	
		void			SetValue(const zenRes::zShaderParameter& _Value);		
		void			SetValue(const zHash32& _hParamName, const float& _fValue);
		void			SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void			SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void			SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture );
		void			SetValue(const zHash32& _hTextureName, GfxSamplerRef _rSampler );
		void			SetValue(const zHash32& _hTextureName, GfxTexture2DRef _rTexture, GfxSamplerRef _rSampler );		
		//------------------------------------------------------------------------------------------------
	};
}

#endif
