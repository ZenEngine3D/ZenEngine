#pragma once
#ifndef __zenApi_Core_ResGfxBinding_h__
#define __zenApi_Core_ResGfxBinding_h__

namespace zen { namespace zenRes {	
	
	class zGfxRenderPass; //! forward declare

	ZENClassResourceRefDeclare(zGfxShaderBinding, zenConst::keResType_GfxShaderBinding)
	public:				
		static zGfxShaderBinding						Create(const zGfxShaderVertex& _VertexShader);
		static zGfxShaderBinding						Create(const zGfxShaderVertex& _VertexShader, const zGfxShaderPixel& _PixelShader);

		//const zArrayStatic<zenRes::zGfxShaderParamDef>&	GetShaderParamDef()const;
		void											CreateShaderParam(zArrayStatic<zenRes::zGfxShaderParam>& _aShaderParamOut)const;
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
//		void							Draw(zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<class zCommand>& _aDrawcallsOut, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0, 0, 0xFFFF, 0xFFFF));
		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArrayBase<const zShaderParameter*>& _aValues);	
		void							SetValue(const zHash32& _hParamName, const float& _fValue);
		void							SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void							SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture );
		void							SetValue(const zHash32& _hTextureName, const zGfxSampler& _rSampler );
		void							SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture, const zGfxSampler& _rSampler );
		
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zGfxShaderParam>& _arShaderParam, const zArrayBase<zShaderTexture>& _aTexture, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst=0); //! @todo clean remove the texture binding from create?
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zGfxShaderParam>& _arShaderParam, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0);
		static zGfxMeshStrip			Create(const zGfxVertex& _rVertexBuffer, const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0);
	};

	ZENClassResourceRefDeclare(zGfxMesh, zenConst::keResType_GfxMesh)
	public:
//		void							Draw(zGfxRenderPass& _rRenderpass, float _fPriority, zArrayDynamic<class zCommand>& _aDrawcallsOut, const zVec4U16& _vScreenScissor = zVec4U16(0, 0, 0xFFFF, 0xFFFF)); //! @todo clean move output array first
		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArrayBase<const zShaderParameter*>& _aValues);	
		void							SetValue(const zHash32& _hParamName, const float& _fValue);
		void							SetValue(const zHash32& _hParamName, const zVec2F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec3F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zVec4F& _vValue);
		void							SetValue(const zHash32& _hParamName, const zenMath::Matrix& _matValue);
		void							SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture );
		void							SetValue(const zHash32& _hTextureName, const zGfxSampler& _rSampler );
		void							SetValue(const zHash32& _hTextureName, const zGfxTexture2d& _rTexture, const zGfxSampler& _rSampler );

		static zGfxMesh					Create(const zArrayBase<zGfxMeshStrip>& _aMeshStrip);
	};
	
}} // namespace zen, zenRes

#include "zenCoreResGfxBinding.inl"

#endif

