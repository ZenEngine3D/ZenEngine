#pragma once
#ifndef __zCore_Gfx_Renderer_Drawcall_DX12_h__
#define __zCore_Gfx_Renderer_Drawcall_DX12_h__

namespace zcGfx
{

class CommandUpdateIndexDX12 : public Command
{
zenClassDeclare(CommandUpdateIndexDX12, Command)
public:
	static zEngineRef<Command>		Create( const zcRes::GfxIndexRef& _rIndex, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void					Invoke();

protected:	
	zcRes::GfxIndexRef				mrIndex;
	DirectXComRef<ID3D12Resource>	mrUpdateData;
	zUInt							muOffset;
	zUInt							muSize;
};

class CommandUpdateBufferDX12 : public Command
{
zenClassDeclare(CommandUpdateBufferDX12, Command)
public:
	static zEngineRef<Command>		Create( const zcRes::GfxBufferRef& _rBuffer, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void					Invoke();

protected:	
	zcRes::GfxBufferRef				mrBuffer;
	DirectXComRef<ID3D12Resource>	mrUpdateData;
	zUInt							muOffset;
	zUInt							muSize;
};

}

#endif
