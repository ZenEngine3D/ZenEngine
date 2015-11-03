#pragma once
#ifndef __zenApi_Core_ResGfxBinding_h__
#define __zenApi_Core_ResGfxBinding_h__

namespace zen { namespace zenRes {	
	
	class zGfxRenderPass; //! forward declare

	ZENClassResourceRefDeclare(zGfxShaderBinding, zenConst::keResType_GfxShaderBinding)
	public:		
		static zGfxShaderBinding		Create(const zArrayBase<zGfxShader>& _aShader);		
	};

	//! @todo Clean: remove this from api
	ZENClassResourceRefDeclare(zGfxInputStream, zenConst::keResType_GfxInputStream) };
	
	ZENClassResourceRefDeclare(zGfxView, zenConst::keResType_GfxView)
	public:
		zVec2U16						GetDim()const;
	};
	
	ZENClassResourceRefDeclare(zGfxWindow, zenConst::keResType_GfxWindow)
	public:
		void							FrameBegin();
		void							FrameEnd();
		zGfxRenderTarget				GetBackbuffer();
		void							Resize(const zVec2U16& _vSize);		
		bool							PerformResize();
		static zGfxWindow				Create( HWND _WindowHandle );
	};
		
	ZENClassResourceRefDeclare(zGfxMeshStrip, zenConst::keResType_GfxMeshStrip)
	public:
		void							Draw(zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<class zGfxDrawcall>& _aDrawcallsOut);
		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArrayBase<const zShaderParameter*>& _aValues);	
		void							SetValue(const zHash32& _hParamName, const float& _fValue);
		void							SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void							SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture );
		void							SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler );
		void							SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture, zGfxSampler _rSampler );
		
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, const zArrayBase<zShaderTexture>& _aTexture, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
	};

	ZENClassResourceRefDeclare(zGfxMesh, zenConst::keResType_GfxMesh)
	public:
		void							Draw(zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<class zGfxDrawcall>& _aDrawcallsOut);
		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArrayBase<const zShaderParameter*>& _aValues);	
		void							SetValue(const zHash32& _hParamName, const float& _fValue);
		void							SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void							SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture );
		void							SetValue(const zHash32& _hTextureName, zGfxSampler _rSampler );
		void							SetValue(const zHash32& _hTextureName, zGfxTexture2d _rTexture, zGfxSampler _rSampler );

		static zGfxMesh					Create(const zArrayBase<zGfxMeshStrip>& _aMeshStrip);
	};
	
}} // namespace zen, zenRes

#include "zenCoreResGfxBinding.inl"

#endif

