#include "zcCore.h"

namespace zcRes
{	

zU8* GfxIndex::Lock()
{
	zenAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");

	//! @todo Urgent lock type and copy data if needed
	//! @todo Urgent Use ring buffer for this	
	//! @todo Urgent	Design MemBuffer class to handle data copy between cpu/gpu
	//					Ref: https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf
	mpLockData = zenNewDefault zU8[ maIndices.SizeMem() ];		
	return mpLockData;
}

void GfxIndex::Unlock(const zenGfx::zContext& _rContext)
{
	//! @todo Urgent Update Cpu copy at frame end
	zenAssertMsg(mpLockData!=nullptr, "Need to lock buffer before unlocking it");
	zEngineRef<zcGfx::Command> rCommand = zcGfx::CommandUpdateIndex::Create( this, mpLockData );
	mpLockData							= nullptr;
	_rContext->AddCommand(rCommand.Get());
}

}
