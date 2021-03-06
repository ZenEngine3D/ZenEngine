#pragma once

//=================================================================================================
//! @class		WindowInputState
//-------------------------------------------------------------------------------------------------
//! @brief		Received OS input updated, and keep track of their states
//! @details	
//=================================================================================================
struct WindowInputState
{
public:
enum eMouseButton
{
	keMouseBtn_Left, 
	keMouseBtn_Right,
	keMouseBtn_Middle,
	keMouseBtn__Count,
	keMouseBtn__Invalid,
};
	void							Reset(zU8 _uMaxKeyProcessed);

	zVec2S16						mvMousePos			= zVec2S16(0,0);	
	float							mfMouseWheel		= 0;
	float							mfMouseWheelDelta	= 0;
	std::bitset<keMouseBtn__Count>	mbIsMouseDown;
	std::bitset<keMouseBtn__Count>	mbIsMousePressed;
	std::bitset<keMouseBtn__Count>	mbIsMouseReleased;
	std::bitset<255>				mbIsKeyDown;
	std::bitset<255>				mbIsKeyPressed;
	std::bitset<255>				mbIsKeyReleased;
	zArrayDyn<zU16>				maCharacterPressed;
};

#include zenHeaderPlatform( zeWndViewport )
