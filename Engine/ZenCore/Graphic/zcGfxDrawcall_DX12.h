#pragma once

namespace zcGfx 
{
//==================================================================================================
// Generic Command supported on all platforms
//==================================================================================================
class CommandDraw_DX12 : public CommandDraw
{ 
zenClassDeclare(CommandDraw_DX12, CommandDraw)
public: 
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF));	
	virtual void					Invoke(GPUContext& _Context);
	zEngineRef<PSO_DX12>			mrPSO;
};

class CommandClearColor_DX12 : public CommandClearColor
{ 
zenClassDeclare(CommandClearColor_DX12, CommandClearColor)
public: 
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTColor, const zVec4F& _vRGBA,  const zColorMask& _ColorMask=zenConst::kColorMaskRGBA, const zVec2S16& _vOrigin=zVec2S16(0,0), const zVec2U16& _vDim=zVec2U16(0,0) );
	virtual void					Invoke(GPUContext& _Context); 
};

class CommandClearDepthStencil_DX12 : public CommandClearDepthStencil
{ 
zenClassDeclare(CommandClearDepthStencil_DX12, CommandClearDepthStencil)
public: 
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxTarget2DRef& _rRTDepth, bool _bClearDepth, float _fDepthValue=1.f, bool _bClearStencil=false, zU8 _uStencilValue=128);
	virtual void					Invoke(GPUContext& _Context); 
};

//==================================================================================================
// Platform specific Commands
//==================================================================================================
class CommandUpdateIndex_DX12 : public Command
{
zenClassDeclare(CommandUpdateIndex_DX12, Command)
public:
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void					Invoke(GPUContext& _Context);

protected:	
	zcRes::GfxIndexRef				mrIndex;
	DirectXComRef<ID3D12Resource>	mrResourceUpload;
	zUInt							muOffset;
	zUInt							muSize;
};

class CommandUpdateBuffer_DX12 : public Command
{
zenClassDeclare(CommandUpdateBuffer_DX12, Command)
public:
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void					Invoke(GPUContext& _Context);

protected:	
	zcRes::GfxBufferRef				mrBuffer;
	DirectXComRef<ID3D12Resource>	mrResourceUpload;
	zUInt							muOffset;
	zUInt							muSize;
};

class CommandUpdateTexture_DX12 : public Command
{
zenClassDeclare(CommandUpdateTexture_DX12, Command)
public:
	static zEngineRef<Command>		Add(const zenGfx::zScopedDrawlist& _rContext, const zcRes::GfxTexture2DRef& _rTexture, zUInt _uSize); //! @todo 2 Support mipmap
	virtual void					Invoke(GPUContext& _Context);

protected:	
	zcRes::GfxTexture2DRef			mrTexture;
	DirectXComRef<ID3D12Resource>	mrResourceUpload;
	zUInt							muSize;
};

#if 0
class CommandUpdateResourceBuffer_DX12 : public Command
{
zenClassDeclare(CommandUpdateResourceBuffer_DX12, Command)
public:
	static zEngineRef<Command>		Create(const zcRes::ResourceAnyRef& _rResource, zcGfx::GpuBuffer& _Buffer); 
	virtual void					Invoke(GPUContext& _Context);

protected:	
	zcRes::ResourceAnyRef			mrResource;				//!< Here to make sure resource stays alive by inc ref count	
	DirectXComRef<ID3D12Resource>	mrUpload;
	zcGfx::GpuBuffer*				mpGPUBuffer	= nullptr;
	zUInt							muSize		= 0;
	zUInt							muOffset	= 0;
};
#endif
} 


