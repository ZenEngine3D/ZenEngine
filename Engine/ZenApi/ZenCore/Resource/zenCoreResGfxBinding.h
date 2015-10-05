#pragma once
#ifndef __zenApi_Core_ResGfxBinding_h__
#define __zenApi_Core_ResGfxBinding_h__

namespace zen { namespace zenRes {	

	ZENClassResourceRefDeclare(zGfxShaderBinding, zenConst::keResType_GfxShaderBinding)
	public:		
		static zGfxShaderBinding		Create(const zArrayBase<zGfxShader>& _aShader);
		static zGfxShaderBinding		Create(const zGfxShader* _pShaders, zUInt _uCount);
	};

	//! @todo Clean: remove this from api
	ZENClassResourceRefDeclare(zGfxInputStream, zenConst::keResType_GfxInputStream) };

	ZENClassResourceRefDeclare(zGfxView, zenConst::keResType_GfxView)
	public:
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

	ZENClassResourceRefDeclare(zGfxRenderPass, zenConst::keResType_GfxRenderPass)
	public:
		struct ViewConfig
		{
			zGfxTexture2d	mTargetSurface;
			zGfxStateBlend	mStateBlend;
			zVec2S16		mvOrigin;
			zVec2U16		mvDim;
		};
		static zGfxRenderPass			Create(const zString& _zStageName, const zGfxStateBlend& _rStateBlend, const zGfxStateDepthStencil&	_rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster, const zGfxView& _rView);
		static zGfxRenderPass			Create(const zString& _zStageName, const ViewConfig& _rView, const zGfxTexture2d& _rTargetDepth, const zGfxStateDepthStencil& _rStateDepthStencil, const zGfxStateRasterizer& _rStateRaster);
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
		static zGfxMesh					Create(const zGfxMeshStrip* _aMeshStrip, zUInt _uMeshStripCount);
	};
	
}} // namespace zen, zenRes


//! @todo Urgent, this is not a resource, should be moved elsewhere
namespace zcGfx  { class Drawcall; }	//Forward declare
namespace zen { namespace zenRes 
{	
	class zGfxDrawcall : public zEngineRef<zcGfx::Drawcall>
	{
	ZENClassDeclare(zGfxDrawcall, zEngineRef<zcGfx::Drawcall>);
	public:			
										zGfxDrawcall();
										zGfxDrawcall(const zGfxDrawcall& _Copy);		
										zGfxDrawcall(zcGfx::Drawcall* _Drawcall);

		zGfxDrawcall&					operator=(const zGfxDrawcall& _Copy);
		zGfxDrawcall&					operator=(zcGfx::Drawcall* _pCopy);
		
		static zGfxDrawcall				Create(const zGfxRenderPass& _rRenderPass, const zGfxMeshStrip&	_rMeshStrip, float _fPriority);
		static void						Submit(zArrayDynamic<zenRes::zGfxDrawcall>& _aDrawcalls);
	};
	
}}
#include "zenCoreResGfxBinding.inl"

#endif

