#pragma once
#ifndef __zenApi_Base_Constant_BuildDefines_h__
#define __zenApi_Base_Constant_BuildDefines_h__

//=================================================================================================
//! @file		APIConstBuildDefines.h
//! @brief		Every Build Defines set to 0 here, each platform will set the one supported to 1
//! @detail
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//! @brief Some build defines config
//-------------------------------------------------------------------------------------------------
#define ZEN_MEMORYDEBUG_ON				0

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
//! @brief Debuging settings
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
	#define ZENDbgCode( _Code_ )			_Code_
#else
	#define ZENDbgCode( _Code_ )
#endif

//=================================================================================================
//! Support for multi-platform file include
//=================================================================================================
#define ZENComma								,															//Useful to insert a comma, inside a macro call, without interpreting it as a new macro parameter
#define ZENStringDefineB(_S_)					#_S_														//Convert parameter into a string
#define ZENStringDefine(_S_)					ZENStringDefineB(_S_)										//Convert content of a define into a string
#define ZENFilenameB(PREFIX, SUFFIX, EXT)		PREFIX##_##SUFFIX##EXT										//Used to generate platform based filename by others macros
#define ZENFilename(PREFIX, SUFFIX, EXT)		ZENFilenameB(PREFIX, SUFFIX, EXT )							//Used to generate platform based filename by others macros

#define ZENHeaderFile(HEADERNAME, DEFINE)		ZENStringDefine( ZENFilename(HEADERNAME, DEFINE, .h) )		//!< Header filename for current Platform type
#define ZENInlineFile(HEADERNAME, DEFINE)		ZENStringDefine( ZENFilename(HEADERNAME, DEFINE, .inl) )	//!< Header filename for current Platform type

#define ZENHeaderPlatform(HEADERNAME)			ZENHeaderFile(HEADERNAME,	ZEN_PLATFORM)					//!< Header filename for current Platform type
#define ZENInlinePlatform(HEADERNAME)			ZENInlineFile(HEADERNAME,	ZEN_PLATFORM)					//!< Inline Header filename for current Platform type
#define ZENClassPlatform(CLASSNAME)				ZENDefineStich3(CLASSNAME, _, ZEN_PLATFORM)					//!< Header filename for current Platform type

#define ZENHeaderRenderer(HEADERNAME)			ZENHeaderFile(HEADERNAME, ZEN_RENDERER)						//!< Header filename for current Renderer type
#define ZENInlineRenderer(HEADERNAME)			ZENInlineFile(HEADERNAME, ZEN_RENDERER)						//!< Inline Header filename for current Renderer type
#define ZENClassRenderer(CLASSNAME)				ZENDefineStich3(CLASSNAME, _, ZEN_RENDERER)					//!< Header filename for current Platform type

#include ZENHeaderPlatform(zenBaseConstBuildDefines)
#include ZENHeaderRenderer(zenBaseConstBuildDefines)

#endif
