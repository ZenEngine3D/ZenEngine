#include "zcCore.h"

namespace zcRes
{	

zU8* GfxIndex::Lock()
{
	ZENAssertMsg(mpLockData==nullptr, "Need to unlock buffer before locking it again");
	
	//! @todo Urgent lock type and copy data if needed
	//! @todo Urgent Use ring buffer for this
	//! @todo Urgent	Design MemBuffer class to handle data copy between cpu/gpu
	//					Ref: https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf
	mpLockData = zenNewDefault zU8[ Super::GetIndices().SizeMem() ];	
	return mpLockData;
}

void GfxIndex::Unlock(const zenGfx::zContext& _rContext)
{
	ZENAssertMsg(mpLockData==nullptr, "Need to lock buffer before unlocking it");
	zEngineRef<zcGfx::Command> rCommand = zcGfx::CommandUpdateIndex::Create( this, mpLockData );
	_rContext->AddCommand(rCommand.Get());
	mpLockData = nullptr;
	
	mData->maIndices;
}
	
}