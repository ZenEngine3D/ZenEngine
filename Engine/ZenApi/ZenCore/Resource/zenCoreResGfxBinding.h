#pragma once
#ifndef __zenApi_Core_ResGfxBinding_h__
#define __zenApi_Core_ResGfxBinding_h__

namespace zen { namespace zenRes {	

	ZENClassResourceRefDeclare(zGfxSampler, zenConst::keResType_GfxSampler)
	public:
		static zGfxSampler				Create( zenConst::eTextureFiltering _eFilterMin=zenConst::keTexFilter_Bilinear, zenConst::eTextureFiltering _eFilterMag=zenConst::keTexFilter_Bilinear, zenConst::eTextureWrap _eWrapU=zenConst::keTexWrap_Repeat, zenConst::eTextureWrap _eWrapV=zenConst::keTexWrap_Repeat, float _fLodBias=0, const zVec4F& _vBorderColor=zVec4F(0,0,0,1) );
	};

	ZENClassResourceRefDeclare(zGfxStateBlend, zenConst::keResType_GfxBlend)
	public:
		static zGfxStateBlend			Create( zenType::zBlendDesc::zRTBlendDesc* _pxBlendDesc=nullptr, zU8 _uRenderTargets = 0, bool _bAlphaToCoverageEnable = false, bool _bIndependentBlendEnable = false );
	};

	ZENClassResourceRefDeclare(zGfxStateDepthStencil, zenConst::keResType_GfxDepthStencil)
	public:
		static zGfxStateDepthStencil	Create( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zU8 _uStencilReadMask = 0xFF, zU8 _uStencilWriteMask = 0xFF, zenConst::eComparisonFunc _eDepthFunc = zenConst::keComparisonFunc_Always, zenType::zDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::zDepthStencilDesc::DepthStencilOp(), zenType::zDepthStencilDesc::DepthStencilOp _xBackFace = zenType::zDepthStencilDesc::DepthStencilOp() );
	};

	ZENClassResourceRefDeclare(zGfxStateRasterizer, zenConst::keResType_GfxRasterizer)
	public:
		static zGfxStateRasterizer		Create( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, zenConst::eCullMode _eCullMode = zenConst::keCullMode_None, zI32 _iDepthBias = 0, float _fDepthBiasClamp = 0.0f, float _fSlopeScaledDepthBias = 0.0f );
	};

	ZENClassResourceRefDeclare(zGfxShaderBinding, zenConst::keResType_GfxShaderBinding)
	public:		
		static zGfxShaderBinding		Create(const zArrayBase<zenRes::zGfxShader>& _aShader);
		static zGfxShaderBinding		Create(const zenRes::zGfxShader* _pShaders, zUInt _uCount);
	};

	//! @todo Clean: remove this from api
	ZENClassResourceRefDeclare(zGfxInputStream, zenConst::keResType_GfxInputStream) };

	ZENClassResourceRefDeclare(zGfxMeshStrip, zenConst::keResType_GfxMeshStrip)
	public:
		void							RenderMeshStrip();
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
		
		static zGfxMeshStrip			Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, const zArrayBase<zShaderTexture>& _aTexture, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
		static zGfxMeshStrip			Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArrayBase<zResID>& _aShaderParamID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
		static zGfxMeshStrip			Create(const zGfxVertex& _VertexBuffer, const zGfxIndex& _IndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF);
	};

	ZENClassResourceRefDeclare(zGfxMesh, zenConst::keResType_GfxMesh)
	public:
		void							RenderMesh();
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
		static zGfxMesh					Create(const zGfxMeshStrip* _aMeshStrip, zUInt _uMeshStripCount);
	};

	ZENClassResourceRefDeclare(zGfxView, zenConst::keResType_GfxView)
	public:
		void							ActivateView();
		void							Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, zU8 _uStencil=0 );
		zVec2U16						GetDim()const;
		
		static zGfxView					Create( const zGfxRenderTarget& _RTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
		static zGfxView					Create( const zArrayBase<zGfxRenderTarget>& _aRTColor, const zGfxRenderTarget& _RTDepth, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
	};

	ZENClassResourceRefDeclare(zGfxWindow, zenConst::keResType_GfxWindow)
	public:
		void							FrameBegin();
		void							FrameEnd();
		zGfxView						GetBackbuffer();
		void							Resize(const zVec2U16& _vSize);

		static zGfxWindow				Create( HWND _WindowHandle );
	};

}} // namespace zen, zenRes

#endif

