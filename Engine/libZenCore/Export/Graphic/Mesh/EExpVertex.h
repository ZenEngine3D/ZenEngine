#pragma once
#ifndef __LibExport_Graphics_SerialVertex_h__
#define __LibExport_Graphics_SerialVertex_h__

namespace EExp
{
	class SerialVertex_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialVertex_Base,EExp::ExportItem)
	public:
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zArrayStatic<zenRes::zGfxVertex::Stream>	maStreams;
			zFlagResUse									mResourceUse;
		};
	};
	
	zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zFlagResUse _ResourceUse);
}

#include "EExpVertexDX11.h"

#endif