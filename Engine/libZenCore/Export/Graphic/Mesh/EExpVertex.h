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
		//! @todo replace awu32 flag by generic flag class
		enum eCreationFlags
		{
			keFlag_Static	= 1<<0,		// Never updated once created
			keFlag_Dynamic	= 1<<1,		// Can be updated once per frame
			keFlag_Volatile	= 1<<2,		// Can be updated multiple time per frame
		};

		
		struct ExportInfo : public EExp::ExportInfoBase
		{			
			zArrayStatic<zenRes::zGfxVertex::Stream>	maStreams;
			zU32										muCreationFlags;
		};
	};
	
	zResID CreateGfxVertex(const zArrayBase<zenRes::zGfxVertex::Stream>& _aStreams, zU32 _uCreationFlags);
}

#include "EExpVertexDX11.h"

#endif