#pragma once
#ifndef __zenCore_ResourceGfxBinding_h__
#define __zenCore_ResourceGfxBinding_h__

namespace zen { namespace awres {	

	AWClassResourceRefDeclare(awGfxSampler, awconst::keResType_GfxSampler)
	public:
		static awGfxSampler			Create( awconst::eTextureFiltering _eFilterMin=awconst::keTexFilter_Bilinear, awconst::eTextureFiltering _eFilterMag=awconst::keTexFilter_Bilinear, awconst::eTextureWrap _eWrapU=awconst::keTexWrap_Repeat, awconst::eTextureWrap _eWrapV=awconst::keTexWrap_Repeat, float _fLodBias=0, const zenVec4F& _vBorderColor=zenVec4F(0,0,0,1) );
	};

	AWClassResourceRefDeclare(awGfxBlend, awconst::keResType_GfxBlend)
	public:
		static awGfxBlend			Create( zenType::awBlendDesc::awRTBlendDesc* _pxBlendDesc=nullptr, zenU8 _uRenderTargets = 0, bool _bAlphaToCoverageEnable = false, bool _bIndependentBlendEnable = false );
	};

	AWClassResourceRefDeclare(awGfxDepthStencil, awconst::keResType_GfxDepthStencil)
	public:
		static awGfxDepthStencil	Create( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, zenU8 _uStencilReadMask = 0xFF, zenU8 _uStencilWriteMask = 0xFF, awconst::eComparisonFunc _eDepthFunc = awconst::keComparisonFunc_Always, zenType::awDepthStencilDesc::DepthStencilOp _xFrontFace = zenType::awDepthStencilDesc::DepthStencilOp(), zenType::awDepthStencilDesc::DepthStencilOp _xBackFace = zenType::awDepthStencilDesc::DepthStencilOp() );
	};

	AWClassResourceRefDeclare(awGfxRasterizer, awconst::keResType_GfxRasterizer)
	public:
		static awGfxRasterizer		Create( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, awconst::eCullMode _eCullMode = awconst::keCullMode_None, zenS32 _iDepthBias = 0, zenF32 _fDepthBiasClamp = 0.0f, zenF32 _fSlopeScaledDepthBias = 0.0f );
	};

	AWClassResourceRefDeclare(awGfxShaderBinding, awconst::keResType_GfxShaderBinding)
	public:		
		//! @todo Convert these function to only use pointer/count
		static awGfxShaderBinding	Create(const zenArrayBase<awres::awGfxShader>& _aShader);
		static awGfxShaderBinding	Create(const awres::awGfxShader* _pShaders, zenUInt _uCount);
	};

	//! @todo remove this from api
	AWClassResourceRefDeclare(awGfxInputStream, awconst::keResType_GfxInputStream) };

	AWClassResourceRefDeclare(awGfxMeshStrip, awconst::keResType_GfxMeshStrip)
	public:
		void						RenderMeshStrip();
		void						SetValue(const awShaderParameter& _Value);			
		void						SetValue(const zenArrayBase<const awShaderParameter*>& _aValues);	
		void						SetValue(const zenHash32& _hParamName, const float& _fValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue);
		void						SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture );
		void						SetValue(const zenHash32& _hTextureName, awGfxSampler _rSampler );
		void						SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler );
		

		//! @todo Replace vertex/index with aw resname
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, const zenArrayBase<awShaderTexture>& _aTexture, zenU32 _uIndexFirst=0, zenU32 _uIndexCount=0xFFFFFFFF);
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const zenArrayBase<zenResID>& _aShaderParamID, zenU32 _uIndexFirst=0, zenU32 _uIndexCount=0xFFFFFFFF);
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, zenU32 _uIndexFirst=0, zenU32 _uIndexCount=0xFFFFFFFF);
	};

	AWClassResourceRefDeclare(awGfxMesh, awconst::keResType_GfxMesh)
	public:
		void						RenderMesh();
		void						SetValue(const awShaderParameter& _Value);			
		void						SetValue(const zenArrayBase<const awShaderParameter*>& _aValues);	
		void						SetValue(const zenHash32& _hParamName, const float& _fValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec2F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec3F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenVec4F& _vValue);
		void						SetValue(const zenHash32& _hParamName, const zenMath::Matrix& _matValue);
		void						SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture );
		void						SetValue(const zenHash32& _hTextureName, awGfxSampler _rSampler );
		void						SetValue(const zenHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler );
		
		static awGfxMesh			Create(const zenArrayBase<awGfxMeshStrip>& _aMeshStrip);
		static awGfxMesh			Create(const awGfxMeshStrip* _aMeshStrip, zenUInt _uMeshStripCount);
	};

	AWClassResourceRefDeclare(awGfxView, awconst::keResType_GfxView)
	public:
		void						ActivateView();
		void						Clear( bool _bClearColor, const zenVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, zenU8 _uStencil=0 );
		zenVec2U16					GetDim()const;
		
		static awGfxView			Create( const awGfxRenderTarget& _RTColor, const awGfxRenderTarget& _RTDepth, const zenVec2U16& _vDim=zenVec2U16(9999,9999), const zenVec2U16& _vOrigin=zenVec2U16(0,0) );
		static awGfxView			Create( const zenArrayBase<awGfxRenderTarget>& _aRTColor, const awGfxRenderTarget& _RTDepth, const zenVec2U16& _vDim=zenVec2U16(9999,9999), const zenVec2U16& _vOrigin=zenVec2U16(0,0) );
	};

	AWClassResourceRefDeclare(awGfxWindow, awconst::keResType_GfxWindow)
	public:
		void						FrameBegin();
		void						FrameEnd();
		awGfxView					GetBackbuffer();
		void						Resize(const zenVec2U16& _vSize);

		static awGfxWindow			Create( HWND _WindowHandle );
	};

}} // namespace zen, awres

#endif

