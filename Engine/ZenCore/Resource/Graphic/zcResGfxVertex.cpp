#include "zcCore.h"

namespace zcRes
{

zU8* GfxVertex::Lock()
{
	zenAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");
	mpLockData = zenNewDefault zU8[ maStream[0].maData.SizeMem() ];		
	return mpLockData;
}

void GfxVertex::Unlock(const zenGfx::zContext& _rContext)
{
	//! @todo Urgent Update Cpu copy at frame end
	zenAssertMsg(mpLockData!=nullptr, "Need to lock buffer before unlocking it");
	zEngineRef<zcGfx::Command> rCommand = zcGfx::CommandUpdateVertex::Create( this, mpLockData );
	mpLockData							= nullptr;
	_rContext->AddCommand(rCommand.Get());
}

}
