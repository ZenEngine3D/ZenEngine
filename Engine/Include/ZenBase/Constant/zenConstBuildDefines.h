#pragma once
#ifndef __LibCore_Api_Constant_BuildDefines_h__
#define __LibCore_Api_Constant_BuildDefines_h__

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
#ifndef AW_ENGINEGAME
	#define AW_ENGINEGAME				0
#endif
#ifndef AW_ENGINETOOL
	#define AW_ENGINETOOL				0
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

#if AW_BUILD_DEBUG || AW_ASSERTON
	#define AWDbgCode( _Code_ )			_Code_
#else
	#define AWDbgCode( _Code_ )
#endif

//=================================================================================================
//! Support for multi-platform file include
//=================================================================================================
#define AWComma									,															//Useful to insert a comma, inside a macro call, without interpreting it as a new macro parameter
#define AWString(_S_)							#_S_														//Convert parameter into a string
#define AWStringDefine(_S_)						AWString(_S_)												//Convert content of a define into a string
#define AWFilenameB(PREFIX, SUFFIX, EXT)		PREFIX##_##SUFFIX##EXT										//Used to generate platform based filename by others macros
#define AWFilename(PREFIX, SUFFIX, EXT)			AWFilenameB(PREFIX, SUFFIX, EXT )							//Used to generate platform based filename by others macros

#define AWHeaderFile(HEADERNAME, DEFINE)		AWStringDefine( AWFilename(HEADERNAME, DEFINE, .h) )		//!< Header filename for current Platform type
#define AWInlineFile(HEADERNAME, DEFINE)		AWStringDefine( AWFilename(HEADERNAME, DEFINE, .inl) )		//!< Header filename for current Platform type

#define AWHeaderPlatform(HEADERNAME)			AWHeaderFile(HEADERNAME,	AW_PLATFORM)					//!< Header filename for current Platform type
#define AWInlinePlatform(HEADERNAME)			AWInlineFile(HEADERNAME,	AW_PLATFORM)					//!< Inline filename for current Platform type
#define AWClassPlatform(CLASSNAME)				AWDefineStich3(CLASSNAME, _, AW_PLATFORM)					//!< Header filename for current Platform type

#define AWHeaderRenderer(HEADERNAME)			AWHeaderFile(HEADERNAME, AW_RENDERER)						//!< Header filename for current Renderer type
#define AWInlineRenderer(HEADERNAME)			AWInlineFile(HEADERNAME, AW_RENDERER)						//!< Inline filename for current Renderer type
#define AWClassRenderer(CLASSNAME)				AWDefineStich3(CLASSNAME, _, AW_RENDERER)					//!< Header filename for current Platform type

#include AWHeaderPlatform(zenConstBuildDefines)
#include AWHeaderRenderer(zenConstBuildDefines)

#endif