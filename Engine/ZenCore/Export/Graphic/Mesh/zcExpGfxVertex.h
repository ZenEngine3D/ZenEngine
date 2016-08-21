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

	class ExportGfxVertex : public zenRes::zExportData
	{
	ZENClassDeclare(ExportGfxVertex, zenRes::zExportData)
	public:
		class Stream
		{
		public:		
			zArrayStatic<zU8>			maData;				//!< Vertex elements data (pos, uv, etc...)
			zU16						muStride;			//!< Stride between 2 vertex entry
			zU8							muElementStart;		//!< First entry index in stream definition
			zU8							muElementCount;		//!< Number of different vertex element contained in buffer (not number of vertice)
		};
		zArrayStatic<Stream>			maStream;			//!< Vertex Buffer data
		zFlagResUse						mResourceUse;		//!< Creation flags
	};

	zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse);
}

#include "zcExpGfxVertexDX11.h"

#endif