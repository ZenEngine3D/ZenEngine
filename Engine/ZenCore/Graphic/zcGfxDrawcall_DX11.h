#pragma once
#ifndef __zCore_Gfx_Renderer_Drawcall_DX11_h__
#define __zCore_Gfx_Renderer_Drawcall_DX11_h__

namespace zcGfx
{

class CommandUpdateIndexDX11 : public Command
{
zenClassDeclare(CommandUpdateIndexDX11, Command)
public:
	static zEngineRef<Command>	Create( const zcRes::GfxIndexRef& _rIndex, zU8* _pData, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void				Invoke();

protected:	
	zcRes::GfxIndexRef			mrIndex;
	zU8*						mpData;
	zUInt						muOffset;
	zUInt						muSize;
};

class CommandUpdateBufferDX11 : public Command
{
zenClassDeclare(CommandUpdateBufferDX11, Command)
public:
	static zEngineRef<Command>	Create( const zcRes::GfxBufferRef& _rBuffer, zU8* _pUpdateData, zUInt _uOffset=0, zUInt _uSize=0xFFFFFFFFFFFFFFFF);
	virtual void				Invoke();

protected:	
	zcRes::GfxBufferRef			mrBuffer;
	zU8*						mpUpdateData;
	zUInt						muOffset;
	zUInt						muSize;
};

}

#endif
