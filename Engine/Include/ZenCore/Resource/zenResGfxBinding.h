#pragma once
#ifndef __LibEngine_Api_ResourceGfxBinding_h__
#define __LibEngine_Api_ResourceGfxBinding_h__

namespace zen { namespace awres {	

	AWClassResourceRefDeclare(awGfxSampler, awconst::keResType_GfxSampler)
	public:
		static awGfxSampler			Create( awconst::eTextureFiltering _eFilterMin=awconst::keTexFilter_Bilinear, awconst::eTextureFiltering _eFilterMag=awconst::keTexFilter_Bilinear, awconst::eTextureWrap _eWrapU=awconst::keTexWrap_Repeat, awconst::eTextureWrap _eWrapV=awconst::keTexWrap_Repeat, float _fLodBias=0, const awVec4F& _vBorderColor=awVec4F(0,0,0,1) );
	};

	AWClassResourceRefDeclare(awGfxBlend, awconst::keResType_GfxBlend)
	public:
		static awGfxBlend			Create( awtype::awBlendDesc::awRTBlendDesc* _pxBlendDesc=nullptr, awU8 _uRenderTargets = 0, bool _bAlphaToCoverageEnable = false, bool _bIndependentBlendEnable = false );
	};

	AWClassResourceRefDeclare(awGfxDepthStencil, awconst::keResType_GfxDepthStencil)
	public:
		static awGfxDepthStencil	Create( bool _bDepthEnable = false, bool _bDepthWrite = false, bool _bStencilEnable = false, awU8 _uStencilReadMask = 0xFF, awU8 _uStencilWriteMask = 0xFF, awconst::eComparisonFunc _eDepthFunc = awconst::keComparisonFunc_Always, awtype::awDepthStencilDesc::DepthStencilOp _xFrontFace = awtype::awDepthStencilDesc::DepthStencilOp(), awtype::awDepthStencilDesc::DepthStencilOp _xBackFace = awtype::awDepthStencilDesc::DepthStencilOp() );
	};

	AWClassResourceRefDeclare(awGfxRasterizer, awconst::keResType_GfxRasterizer)
	public:
		static awGfxRasterizer		Create( bool _bFrontCounterClockwise = false, bool _bDepthClipEnable = false, bool _bScissorEnable = false, bool _bMultisampleEnable = false, bool _bAntialiasedLineEnable = false, bool _bWireFrame = false, awconst::eCullMode _eCullMode = awconst::keCullMode_None, awS32 _iDepthBias = 0, awF32 _fDepthBiasClamp = 0.0f, awF32 _fSlopeScaledDepthBias = 0.0f );
	};

	AWClassResourceRefDeclare(awGfxShaderBinding, awconst::keResType_GfxShaderBinding)
	public:		
		//! @todo Convert these function to only use pointer/count
		static awGfxShaderBinding	Create(const awArrayBase<awres::awGfxShader>& _aShader);
		static awGfxShaderBinding	Create(const awres::awGfxShader* _pShaders, awUInt _uCount);
	};

	//! @todo remove this from api
	AWClassResourceRefDeclare(awGfxInputStream, awconst::keResType_GfxInputStream) };

	AWClassResourceRefDeclare(awGfxMeshStrip, awconst::keResType_GfxMeshStrip)
	public:
		void						RenderMeshStrip();
		void						SetValue(const awShaderParameter& _Value);			
		void						SetValue(const awArrayBase<const awShaderParameter*>& _aValues);	
		void						SetValue(const awHash32& _hParamName, const float& _fValue);
		void						SetValue(const awHash32& _hParamName, const awVec2F& _vValue);
		void						SetValue(const awHash32& _hParamName, const awVec3F& _vValue);
		void						SetValue(const awHash32& _hParamName, const awVec4F& _vValue);
		void						SetValue(const awHash32& _hParamName, const zenMath::Matrix& _matValue);
		void						SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture );
		void						SetValue(const awHash32& _hTextureName, awGfxSampler _rSampler );
		void						SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler );
		

		//! @todo Replace vertex/index with aw resname
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const awArrayBase<awResourceID>& _aShaderParamID, const awArrayBase<awShaderTexture>& _aTexture, awU32 _uIndexFirst=0, awU32 _uIndexCount=0xFFFFFFFF);
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, const awArrayBase<awResourceID>& _aShaderParamID, awU32 _uIndexFirst=0, awU32 _uIndexCount=0xFFFFFFFF);
		static awGfxMeshStrip		Create(awGfxVertex _VertexBuffer, awGfxIndex _IndexBuffer, awGfxShaderBinding _rShaderBinding, awU32 _uIndexFirst=0, awU32 _uIndexCount=0xFFFFFFFF);
	};

	AWClassResourceRefDeclare(awGfxMesh, awconst::keResType_GfxMesh)
	public:
		void						RenderMesh();
		void						SetValue(const awShaderParameter& _Value);			
		void						SetValue(const awArrayBase<const awShaderParameter*>& _aValues);	
		void						SetValue(const awHash32& _hParamName, const float& _fValue);
		void						SetValue(const awHash32& _hParamName, const awVec2F& _vValue);
		void						SetValue(const awHash32& _hParamName, const awVec3F& _vValue);
		void						SetValue(const awHash32& _hParamName, const awVec4F& _vValue);
		void						SetValue(const awHash32& _hParamName, const zenMath::Matrix& _matValue);
		void						SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture );
		void						SetValue(const awHash32& _hTextureName, awGfxSampler _rSampler );
		void						SetValue(const awHash32& _hTextureName, awGfxTexture2D _rTexture, awGfxSampler _rSampler );
		
		static awGfxMesh			Create(const awArrayBase<awGfxMeshStrip>& _aMeshStrip);
		static awGfxMesh			Create(const awGfxMeshStrip* _aMeshStrip, awUInt _uMeshStripCount);
	};

	AWClassResourceRefDeclare(awGfxView, awconst::keResType_GfxView)
	public:
		void						ActivateView();
		void						Clear( bool _bClearColor, const awVec4F& _vRGBA, bool _bClearDepth=true, float _fDepth=1, bool _bClearStencil=false, awU8 _uStencil=0 );
		awVec2U16					GetDim()const;
		
		static awGfxView			Create( const awGfxRenderTarget& _RTColor, const awGfxRenderTarget& _RTDepth, const awVec2U16& _vDim=awVec2U16(9999,9999), const awVec2U16& _vOrigin=awVec2U16(0,0) );
		static awGfxView			Create( const awArrayBase<awGfxRenderTarget>& _aRTColor, const awGfxRenderTarget& _RTDepth, const awVec2U16& _vDim=awVec2U16(9999,9999), const awVec2U16& _vOrigin=awVec2U16(0,0) );
	};

	AWClassResourceRefDeclare(awGfxWindow, awconst::keResType_GfxWindow)
	public:
		void						FrameBegin();
		void						FrameEnd();
		awGfxView					GetBackbuffer();
		void						Resize(const awVec2U16& _vSize);

		static awGfxWindow			Create( HWND _WindowHandle );
	};

}} // namespace zen, awres

#endif

