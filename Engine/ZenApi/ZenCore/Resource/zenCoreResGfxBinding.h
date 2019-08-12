#pragma once

//! @todo 2 Urgent, this is not a resource, should be moved elsewhere
//Forward declare
namespace zcGfx  { class Command; }										
namespace zen { namespace zenGfx { class zCommandList; }}
namespace zen { namespace zenRes { class zGfxMesh; class zGfxMeshStrip; }}

namespace zen { namespace zenGfx 
{	
	class zCommand : public zEngineRef<zcGfx::Command>
	{
	zenClassDeclare(zCommand, zEngineRef<zcGfx::Command>);
	public:			
								zCommand();
								zCommand(const zCommand& _Copy);		
								zCommand(zcGfx::Command* _Drawcall);
		
		zCommand&				operator=(zcGfx::Command* _pCopy);
		zCommand&				operator=(const zCommand& _Copy);
		bool					zCommand::operator>(const zCommand& _Cmp)const;
		bool					zCommand::operator>=(const zCommand& _Cmp)const;

		static void				DrawMesh			(const zCommandList& _rContext, float _fPriority, const zenRes::zGfxMesh& _rMesh, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF) );
		static void				DrawMesh			(const zCommandList& _rContext, float _fPriority, const zenRes::zGfxMeshStrip&	_rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF) );
		static void				ClearColor			(const zCommandList& _rContext, const zenRes::zGfxTarget2D& _RTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
		static void				ClearDepthStencil	(const zCommandList& _rContext, const zenRes::zGfxTarget2D& _rRTDepthStencil, bool _bClearDepth=true, float _fDepthValue=1, bool _bClearStencil=false, zU8 _uStencilValue=128);
	};	
}}

namespace zen { namespace zenRes {	
	
	class zGfxRenderPass; //! forward declare

	class zGfxShaderBinding : public zcRes::GfxShaderBindingRef
	{
	zenClassDeclare(zGfxShaderBinding, zcRes::GfxShaderBindingRef);
	public:
		using Super::Super;
		using Super::operator=;
	
		static zGfxShaderBinding		Create(const zGfxShaderVertex& _VertexShader);
		static zGfxShaderBinding		Create(const zGfxShaderVertex& _VertexShader, const zGfxShaderPixel& _PixelShader);
		void							CreateShaderParam(zArrayDyn<zenRes::zGfxCBuffer>& _aShaderParamOut)const;
	};

	class zGfxView : public zcRes::GfxViewRef
	{
	zenClassDeclare(zGfxView, zcRes::GfxViewRef);
	public:
		using Super::Super;
		using Super::operator=;
		zVec2U16						GetDim()const;
	};
		
	class zGfxWindow : public zcRes::GfxWindowRef
	{
	zenClassDeclare(zGfxWindow, zcRes::GfxWindowRef);
	public:
		using Super::Super;
		using Super::operator=;

		void							FrameBegin();
		void							FrameEnd();
		zGfxTarget2D					GetBackbuffer();
		void							Resize(const zVec2U16& _vSize);		
		bool							PerformResize();		
	};

	class zGfxMeshStrip : public zcRes::GfxMeshStripRef
	{
	zenClassDeclare(zGfxMeshStrip, zcRes::GfxMeshStripRef);
	public:
		using Super::Super;
		using Super::operator=;

		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArray<const zShaderParameter*>& _aValues);	
		void							SetValue(zHash32 _hParamName, const float& _fValue);
		void							SetValue(zHash32 _hParamName, const zVec2F& _vValue);
		void							SetValue(zHash32 _hParamName, const zVec3F& _vValue);
		void							SetValue(zHash32 _hParamName, const zVec4F& _vValue);
		void							SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue);
		void							SetResource(zHash32 _hTextureName, const zGfxTexture2D& _rTexture, zU16 _uIndex=0 );
		void							SetResource(zHash32 _hSamplerName, const zGfxStateSampler& _rSampler, zU16 _uIndex=0 );
		void							SetResource(zHash32 _hBufferName, const zGfxBuffer& _rBuffer, zU16 _uIndex=0 );

		static zGfxMeshStrip			Create(const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, const zArray<zShaderResource>& _aResources, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst=0);
		static zGfxMeshStrip			Create(const zGfxIndex& _rIndexBuffer, const zGfxShaderBinding& _rShaderBinding, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0);
	};

	class zGfxMesh : public zcRes::GfxMeshRef
	{
	zenClassDeclare(zGfxMesh, zcRes::GfxMeshRef);
	public:
		using Super::Super;
		using Super::operator=;

		void							SetValue(const zShaderParameter& _Value);			
		void							SetValue(const zArray<const zShaderParameter*>& _aValues);	
		void							SetValue(zHash32 _hParamName, const float& _fValue);
		void							SetValue(zHash32 _hParamName, const zVec2F& _vValue);
		void							SetValue(zHash32 _hParamName, const zVec3F& _vValue);
		void							SetValue(zHash32 _hParamName, const zVec4F& _vValue);
		void							SetValue(zHash32 _hParamName, const zenMath::Matrix& _matValue);
		void							SetResource(zHash32 _hTextureName, const zGfxTexture2D& _rTexture, zU16 _uIndex=0 );
		void							SetResource(zHash32 _hSamplerName, const zGfxStateSampler& _rSampler, zU16 _uIndex=0 );
		void							SetResource(zHash32 _hBufferName, const zGfxBuffer& _rBuffer, zU16 _uIndex=0 );

		static zGfxMesh					Create(const zArray<zGfxMeshStrip>& _aMeshStrip);
		static zGfxMesh					Create(std::initializer_list<zGfxMeshStrip> _aMeshStrip);
	};
	
}} // namespace zen, zenRes

#include "zenCoreResGfxBinding.inl"
