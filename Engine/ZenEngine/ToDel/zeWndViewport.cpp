#include "zeEngine.h"

void WindowInputState::Reset(zU8 _uMaxKeyProcessed)
{	
	mbIsKeyPressed.reset();
	mbIsKeyReleased.reset();
	mbIsMousePressed.reset();
	mbIsMouseReleased.reset();
	mfMouseWheelDelta = 0;

	// Copy any keypress not processed this frame, to be processed next one
	if(maCharacterPressed.size() > _uMaxKeyProcessed)
	{
		zArrayDyn<zU16>	maCharacterCopy = maCharacterPressed;
		maCharacterPressed.clear();
		maCharacterPressed.Reserve(16);
		for (zInt idx(_uMaxKeyProcessed), count(maCharacterPressed.size()); idx<count; idx)
			maCharacterPressed.push_back(maCharacterCopy[idx]);
	}
	else
	{
		maCharacterPressed.clear();
		maCharacterPressed.Reserve(16);
	}		
}