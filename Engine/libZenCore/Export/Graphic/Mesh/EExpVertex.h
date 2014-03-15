#pragma once
#ifndef __LibExport_Graphics_SerialVertex_h__
#define __LibExport_Graphics_SerialVertex_h__

namespace EExp
{
	/*
	class VertexElemInfo
	{
	ZENClassDeclareNoParent(VertexElemInfo)
	public:
							VertexElemInfo(){};
							VertexElemInfo(eShaderElementType _eType, zU8 _ElementCount, eShaderSemantic _eSemantic, zU8 _Offset )
							: meType(_eType)
							, meSemantic(_eSemantic)
							, muVectorSize(_ElementCount)
							, muOffset(_Offset)
							{}

		eShaderElementType	meType;
		eShaderSemantic		meSemantic;
		zU8					muVectorSize;
		zU8					muOffset;		
	};

	class VertexStreamInfo
	{
	ZENClassDeclareNoParent(VertexStreamInfo)
	public:
		zArrayStatic<zU8>				maData;
		zArrayStatic<VertexElemInfo>	maElements;
		zU8							muStride;
		VertexStreamInfo& operator=(const VertexStreamInfo& _Copy)
		{
			maData		= _Copy.maData;
			maElements	= _Copy.maElements;
			muStride	= _Copy.muStride;
			return *this;
		}
	};
	*/

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