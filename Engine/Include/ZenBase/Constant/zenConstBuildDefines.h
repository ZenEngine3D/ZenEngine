#pragma once
#ifndef __LibZenBase_Api_Constant_BuildDefines_h__
#define __LibZenBase_Api_Constant_BuildDefines_h__

//=================================================================================================
//! @file		APIConstBuildDefines.h
//! @brief		Every Build Defines set to 0 here, each platform will set the one supported to 1
//! @detail
//=================================================================================================

//-------------------------------------------------------------------------------------------------
//! @brief Some build defines config
//-------------------------------------------------------------------------------------------------
#define AW_MEMORYDEBUG					0

//-------------------------------------------------------------------------------------------------
//! @brief Define configurations not currently built
//-------------------------------------------------------------------------------------------------
#ifndef AW_BUILD_DEBUG
	#define AW_BUILD_DEBUG				0
#endif
#ifndef AW_BUILD_RELEASE
	#define AW_BUILD_RELEASE			0
#endif
#ifndef AW_BUILD_FINAL
	#define	AW_BUILD_FINAL				0
#endif
#ifndef ZEN_ENGINEGAME
	#define ZEN_ENGINEGAME				0
#endif
#ifndef ZEN_ENGINETOOL
	#define ZEN_ENGINETOOL				0
#endif
#ifndef AW_PLATFORM_PC
	#define AW_PLATFORM_PC				0
#endif
#ifndef AW_RENDERER_DX11
	#define AW_RENDERER_DX11			0
#endif

//-------------------------------------------------------------------------------------------------
//! @brief Debuging settings
//-------------------------------------------------------------------------------------------------
#if defined( AW_BUILD_DEBUG )
	#define AW_DEBUGINFOON		1
	#define AW_ASSERT_BREAKON	1
	#define AWASSERT_MSG_ON		0
#elif defined( AW_BUILD_RELEASE )
	#define AW_DEBUGINFOON		1
	#define AW_ASSERT_BREAKON	0
	#define AWASSERT_MSG_ON		0
#elif defined( AW_BUILD_FINAL )
	#define AW_DEBUGINFOON		0
	#define AW_ASSERT_BREAKON	0
	#define AWASSERT_MSG_ON		0
#endif
#define AW_ASSERTON (AW_ASSERT_BREAKON || AWASSERT_MSG_ON)

#if AW_DEBUGINFOON || AW_ASSERTON
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

#define ZENHeaderFile(HEADERNAME, DEFINE)		ZENStringDefine( ZENFilename(HEADERNAME, DEFINE, .h) )		//!< AllocHeader filename for current Platform type
#define ZENInlineFile(HEADERNAME, DEFINE)		ZENStringDefine( ZENFilename(HEADERNAME, DEFINE, .inl) )		//!< AllocHeader filename for current Platform type

#define ZENHeaderPlatform(HEADERNAME)			ZENHeaderFile(HEADERNAME,	AW_PLATFORM)					//!< AllocHeader filename for current Platform type
#define ZENInlinePlatform(HEADERNAME)			ZENInlineFile(HEADERNAME,	AW_PLATFORM)					//!< Inline filename for current Platform type
#define ZENClassPlatform(CLASSNAME)				ZENDefineStich3(CLASSNAME, _, AW_PLATFORM)					//!< AllocHeader filename for current Platform type

#define ZENHeaderRenderer(HEADERNAME)			ZENHeaderFile(HEADERNAME, AW_RENDERER)						//!< AllocHeader filename for current Renderer type
#define ZENInlineRenderer(HEADERNAME)			ZENInlineFile(HEADERNAME, AW_RENDERER)						//!< Inline filename for current Renderer type
#define ZENClassRenderer(CLASSNAME)				ZENDefineStich3(CLASSNAME, _, AW_RENDERER)					//!< AllocHeader filename for current Platform type

#include ZENHeaderPlatform(zenConstBuildDefines)
#include ZENHeaderRenderer(zenConstBuildDefines)

#endif