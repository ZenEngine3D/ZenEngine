#pragma once
#ifndef __LibFramework_Window_Viewport_h__
#define __LibFramework_Window_Viewport_h__

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
	std::bitset<keMouseBtn__Count>	mbIsMouseDown;
	std::bitset<255>				mbIsKeyDown;
	std::bitset<255>				mbIsKeyPressed;
	std::bitset<255>				mbIsKeyReleased;
	zArrayDynamic<zU16>				maCharacterPressed;
};

#include ZENHeaderPlatform( zeWndViewport )

#endif
