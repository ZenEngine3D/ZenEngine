#pragma once
#ifndef __zCore_Exp_Gfx_SerialVertex_h__
#define __zCore_Exp_Gfx_SerialVertex_h__

namespace zcExp
{
	struct ExportInfoGfxVertex : public ExportInfoBase
	{
		zArrayStatic<zenRes::zGfxVertex::Stream>	maStreams;
		zFlagResUse									mResourceUse;
	};
	/*
	class SerialVertex_Base : public zcExp::ExportItem
	{
	ZENClassDeclare(SerialVertex_Base,zcExp::ExportItem)
	public:
		struct ExportInfo : public zcExp::ExportInfoBase
		{			
			zArrayStatic<zenRes::zGfxVertex::Stream>	maStreams;
			zFlagResUse									mResourceUse;
		};
	};
	*/
	zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse);
}

#include "zcExpGfxVertexDX11.h"

#endif