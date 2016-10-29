#include "zcCore.h"

namespace zcRes
{	

void* GfxBuffer::Lock()
{
	zenAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");
	mpLockData = zenNewDefault zU8[ muElementCount*muElementStride ];		
	return mpLockData;
}

void GfxBuffer::Unlock(const zenGfx::zContext& _rContext)
{
	//! @todo Urgent Update Cpu copy at frame end
	zenAssertMsg(mpLockData!=nullptr, "Need to lock buffer before unlocking it");
	zEngineRef<zcGfx::Command> rCommand = zcGfx::CommandUpdateBuffer::Create(this, mpLockData );
	mpLockData							= nullptr;
	_rContext->AddCommand(rCommand.Get());
}

}
