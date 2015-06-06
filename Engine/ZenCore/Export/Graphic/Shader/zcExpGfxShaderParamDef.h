#pragma once
#ifndef __zCore_Exp_Gfx_ShaderParamDef_h__
#define __zCore_Exp_Gfx_ShaderParamDef_h__

namespace zcExp
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

	struct ExportInfoGfxShaderParamDef : public ExportInfoBase
	{
		zResID					mParentShaderID;
		zcExp::eShaderParamFreq	meBufferIndex;
		ShaderParamItemInfoMap	mdParameters;
		static zResID 			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	zResID CreateGfxShaderParamDef( zResID _ParentShaderID, zcExp::eShaderParamFreq _eBufferIndex );

}

#include "zcExpGfxShaderParamDefDX11.h"

#endif