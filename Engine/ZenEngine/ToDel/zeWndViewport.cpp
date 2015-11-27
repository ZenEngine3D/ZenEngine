#include "zeEngine.h"

void WindowInputState::Reset(zU8 _uMaxKeyProcessed)
{	
	mbIsKeyPressed.reset();
	mbIsKeyReleased.reset();

	// Copy any keypress not processed this frame, to be processed next one
	if(maCharacterPressed.Count() > _uMaxKeyProcessed)
	{
		zArrayDynamic<zU16>	maCharacterCopy = maCharacterPressed;
		maCharacterPressed.Clear();
		maCharacterPressed.Reserve(16);
		for (zInt idx(_uMaxKeyProcessed), count(maCharacterPressed.Count()); idx<count; idx)
			maCharacterPressed.Push(maCharacterCopy[idx]);
	}
	else
	{
		maCharacterPressed.Clear();
		maCharacterPressed.Reserve(16);
	}		
}