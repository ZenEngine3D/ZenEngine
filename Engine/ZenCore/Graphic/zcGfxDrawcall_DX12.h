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
	static zEngineRef<Command>		Create(	const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, const zVec4U16& _vScreenScissor = zVec4U16(0,0,0xFFFF,0xFFFF));
	virtual void					Invoke(GPUContext& _Context);
	zEngineRef<PSO_DX12>			mrPSO;
};

class CommandClearColor_DX12 : public CommandClearColor
{ 
zenClassDeclare(CommandClearColor_DX12, CommandClearColor)
public: 
	virtual void					Invoke(GPUContext& _Context); 
};

class CommandClearDepthStencil_DX12 : public CommandClearDepthStencil
{ 
zenClassDeclare(CommandClearDepthStencil_DX12, CommandClearDepthStencil)
public: 
	virtual void					Invoke(GPUContext& _Context); 
};

//==================================================================================================
// Platform specific Commands
//==================================================================================================
class CommandUpdateIndex_DX12 : public Command
{
zenClassDeclare(CommandUpdateIndex_DX12, Command)
public:
	static zEngineRef<Command>		Create(const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
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
	static zEngineRef<Command>		Create(const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
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
	static zEngineRef<Command>		Create(const zcRes::GfxTexture2DRef& _rTexture, zUInt _uSize); //! @todo 2 Support mipmap
	virtual void					Invoke(GPUContext& _Context);

protected:	
	zcRes::GfxTexture2DRef			mrTexture;
	DirectXComRef<ID3D12Resource>	mrResourceUpload;
	zUInt							muSize;
};


} 


