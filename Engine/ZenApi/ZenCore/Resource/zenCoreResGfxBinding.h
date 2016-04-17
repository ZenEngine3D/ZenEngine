#pragma once
#ifndef __zenApi_Core_ResGfxBinding_h__
#define __zenApi_Core_ResGfxBinding_h__

//! @todo Urgent, this is not a resource, should be moved elsewhere
//Forward declare
namespace zcGfx  { class Command; }										
namespace zen { namespace zenGfx { class zContext; }}
namespace zen { namespace zenRes { class zGfxMesh; class zGfxMeshStrip; }}

namespace zen { namespace zenGfx 
{	
	class zCommand : public zEngineRef<zcGfx::Command>
	{
	ZENClassDeclare(zCommand, zEngineRef<zcGfx::Command>);
	public:			
								zCommand();
								zCommand(const zCommand& _Copy);		
								zCommand(zcGfx::Command* _Drawcall);
		
		zCommand&				operator=(zcGfx::Command* _pCopy);
		zCommand&				operator=(const zCommand& _Copy);
		bool					zCommand::operator>(const zCommand& _Cmp)const;
		bool					zCommand::operator>=(const zCommand& _Cmp)const;

		static void				DrawMesh			(const zContext& _rContext, float _fPriority, const zenRes::zGfxMesh& _rMesh, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF) );
		static void				DrawMesh			(const zContext& _rContext, float _fPriority, const zenRes::zGfxMeshStrip&	_rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF) );
		static void				ClearColor			(const zContext& _rContext, const zenRes::zGfxRenderTarget& _RTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
		static void				ClearDepthStencil	(const zContext& _rContext, const zenRes::zGfxRenderTarget& _rRTDepthStencil, bool _bClearDepth=true, float _fDepthValue=1, bool _bClearStencil=false, zU8 _uStencilValue=128);
	};	
}}

namespace zen { namespace zenRes {	
	
	class zGfxRenderPass; //! forward declare

	ZENClassResourceRefDeclare(zGfxShaderBinding, zenConst::keResType_GfxShaderBinding)
	public:				
		static zGfxShaderBinding						Create(const zGfxShaderVertex& _VertexShader);
		static zGfxShaderBinding						Create(const zGfxShaderVertex& _VertexShader, const zGfxShaderPixel& _PixelShader);
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

