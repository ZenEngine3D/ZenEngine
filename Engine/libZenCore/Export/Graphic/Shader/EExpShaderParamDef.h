#pragma once
#ifndef __LibExport_Graphics_ShaderParamDef_h__
#define __LibExport_Graphics_ShaderParamDef_h__

namespace EExp
{

class ShaderParamItemInfo
{
ZENClassDeclareNoParent(ShaderParamItemInfo)
public:	
	zU32	muOffset;				//!< Offset of value in constant buffer or register index (maximum 64k offset)
	zU32	muSize			: 13;	//!< Parameter entire memory size (maximum 8192 bytes)
	zU32	muColCount		: 4;	//!< Number of element in vector (1 if scalar)
	zU32	muRowCount		: 4;	//!< Number of lines in matrix (1 if scalar or vector)
	zU32	muArrayCount	: 10;	//!< Number of entries in array (support maximum 1024, 0 if not an array)
	zU32	muArrayItemSize	: 10;	//!< Size of 1 array entry including padding (maximum 1024 bytes)
	zU32	meType			: 6;	//!< eShaderElementType entry (float,int,...)
	zU32	mbInUse			: 1;	//!< Shader parameter used in shader or not	
};
typedef zMap<ShaderParamItemInfo>::Key32 ShaderParamItemInfoMap;

class SerialShaderParamDef_Base : public EExp::ExportItem
{
ZENClassDeclare(SerialShaderParamDef_Base, EExp::ExportItem)
public:
	struct ExportInfo : public EExp::ExportInfoBase
	{				
		zResID			mParentShaderID;
		EExp::eShaderParamFreq	meBufferIndex;
	};

	ShaderParamItemInfoMap		mdParameters;
};

zResID CreateGfxShaderParamDef( zResID _ParentShaderID, EExp::eShaderParamFreq _eBufferIndex );

}

#include "EExpShaderParamDefDX11.h"

#endif