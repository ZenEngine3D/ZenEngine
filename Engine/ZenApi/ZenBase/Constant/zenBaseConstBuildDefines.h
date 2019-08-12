#pragma once

//=================================================================================================
//! @file		APIConstBuildDefines.h
//! @brief		Every Build Defines set to 0 here, each platform will set the one supported to 1
//! @detail
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//! @brief Some build defines config
//-------------------------------------------------------------------------------------------------
#define ZEN_MEMORY_FORCE_NATIVE_MALLOC	(0)							//!< Turn off memory system replacement and use built-in *new/malloc*. Slower but usefull to debug with VS memory tracking
#define ZEN_MEMORY_TRACKING				(1 && !ZEN_BUILD_FINAL)		//!< Keeps track of each allocation and write overflow
#define ZEN_MEMORY_TRACKING_DETAILED	(0 && ZEN_MEMORY_TRACKING)	//!< Also keeps track of entire callstack on each allocation (slowdown allocations)
#define ZEN_MEMORY_SUPPORT_CHECKACCESS	(1 && !ZEN_BUILD_FINAL)		//!< Detect out-of-bound memory access or to memory that has been freed already. Requires a lot more memory, so should be used selectively


//-------------------------------------------------------------------------------------------------
//! @brief Define configurations not currently built
//-------------------------------------------------------------------------------------------------
#ifndef ZEN_BUILD_DEBUG
	#define ZEN_BUILD_DEBUG				0
#endif
#ifndef ZEN_BUILD_RELEASE
	#define ZEN_BUILD_RELEASE			0
#endif
#ifndef ZEN_BUILD_FINAL
	#define	ZEN_BUILD_FINAL				0
#endif
#ifndef ZEN_ENGINEGAME
	#define ZEN_ENGINEGAME				0
#endif
#ifndef ZEN_ENGINETOOL
	#define ZEN_ENGINETOOL				0
#endif
#ifndef ZEN_PLATFORM_PC
	#define ZEN_PLATFORM_PC				0
#endif
#ifndef ZEN_RENDERER_DX11
	#define ZEN_RENDERER_DX11			0
#endif

#ifndef ZEN_ENGINELIB
	#define ZEN_ENGINELIB				0
#endif

//-------------------------------------------------------------------------------------------------
//! @brief Debugging settings
//-------------------------------------------------------------------------------------------------
#if defined( ZEN_BUILD_DEBUG )
	#define ZEN_DEBUGINFO_ON		1
	#define ZEN_ASSERT_BREAK_ON		1
	#define ZEN_ASSERT_MSG_ON		0
#elif defined( ZEN_BUILD_RELEASE )
	#define ZEN_DEBUGINFO_ON		1
	#define ZEN_ASSERT_BREAK_ON		0
	#define ZEN_ASSERT_MSG_ON		0
#elif defined( ZEN_BUILD_FINAL )
	#define ZEN_DEBUGINFO_ON		0
	#define ZEN_ASSERT_BREAK_ON		0
	#define ZEN_ASSERT_MSG_ON		0
#endif
#define ZEN_ASSERT_ON (ZEN_ASSERT_BREAK_ON || ZEN_ASSERT_MSG_ON)

#if ZEN_DEBUGINFO_ON || ZEN_ASSERT_ON
	#define zenDbgCode( _Code_ )			_Code_
#else
	#define zenDbgCode( _Code_ )
#endif

//=================================================================================================
//! Support for multi-platform file include
//=================================================================================================
//! @todo 2 This should be only on engine side, no api
#define zenComma								,															//Useful to insert a comma, inside a macro call, without interpreting it as a new macro parameter
#define zenStringDefineB(_S_)					#_S_														//Convert parameter into a string
#define zenStringDefine(_S_)					zenStringDefineB(_S_)										//Convert content of a define into a string
#define zenFilenameB(PREFIX, SUFFIX, EXT)		PREFIX##_##SUFFIX##EXT										//Used to generate platform based filename by others macros
#define zenFilename(PREFIX, SUFFIX, EXT)		zenFilenameB(PREFIX, SUFFIX, EXT )							//Used to generate platform based filename by others macros

#define zenHeaderFile(HEADERNAME, DEFINE)		zenStringDefine( zenFilename(HEADERNAME, DEFINE, .h) )		//!< Header filename for current Platform type
#define zenInlineFile(HEADERNAME, DEFINE)		zenStringDefine( zenFilename(HEADERNAME, DEFINE, .inl) )	//!< Header filename for current Platform type

#define zenHeaderPlatform(HEADERNAME)			zenHeaderFile(HEADERNAME,	ZEN_PLATFORM)					//!< Header filename for current Platform type
#define zenInlinePlatform(HEADERNAME)			zenInlineFile(HEADERNAME,	ZEN_PLATFORM)					//!< Inline Header filename for current Platform type
#define zenClassPlatform(CLASSNAME)				zenDefineStich3(CLASSNAME, _, ZEN_PLATFORM)					//!< Header filename for current Platform type

#define zenHeaderRenderer(HEADERNAME)			zenHeaderFile(HEADERNAME, ZEN_RENDERER)						//!< Header filename for current Renderer type
#define zenInlineRenderer(HEADERNAME)			zenInlineFile(HEADERNAME, ZEN_RENDERER)						//!< Inline Header filename for current Renderer type
#define zenClassRenderer(CLASSNAME)				zenDefineStich3(CLASSNAME, _, ZEN_RENDERER)					//!< Header filename for current Platform type

#include zenHeaderPlatform(zenBaseConstBuildDefines)
#include zenHeaderRenderer(zenBaseConstBuildDefines)
