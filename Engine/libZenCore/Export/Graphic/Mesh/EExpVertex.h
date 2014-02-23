#pragma once
#ifndef __LibExport_Graphics_SerialVertex_h__
#define __LibExport_Graphics_SerialVertex_h__

namespace EExp
{
	/*
	class VertexElemInfo
	{
	AWClassDeclareNoParent(VertexElemInfo)
	public:
							VertexElemInfo(){};
							VertexElemInfo(eShaderElementType _eType, zenU8 _ElementCount, eShaderSemantic _eSemantic, zenU8 _Offset )
							: meType(_eType)
							, meSemantic(_eSemantic)
							, muVectorSize(_ElementCount)
							, muOffset(_Offset)
							{}

		eShaderElementType	meType;
		eShaderSemantic		meSemantic;
		zenU8					muVectorSize;
		zenU8					muOffset;		
	};

	class VertexStreamInfo
	{
	AWClassDeclareNoParent(VertexStreamInfo)
	public:
		zenArrayStatic<zenU8>				maData;
		zenArrayStatic<VertexElemInfo>	maElements;
		zenU8							muStride;
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
	AWClassDeclare(SerialVertex_Base,EExp::ExportItem)
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
			zenArrayStatic<awres::awGfxVertex::Stream>	maStreams;
			zenU32										muCreationFlags;
		};
	};
	
	zenResID CreateGfxVertex(const zenArrayBase<awres::awGfxVertex::Stream>& _aStreams, zenU32 _uCreationFlags);
}

#include "EExpVertexDX11.h"

#endif