#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxVertex_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxVertex_DX11_h__

namespace zcExp
{

class ExportDataGfxVertexDX11 : public ExportDataBase
{
ZENClassDeclare(ExportDataGfxVertexDX11, ExportDataBase)
public:
	class Stream : public zcExp::ISerialize
	{
	ZENClassDeclare(Stream, zcExp::ISerialize)
	public:		
		virtual bool					Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zArrayStatic<zU8>				maData;				//!< Vertex elements data (pos, uv, etc...)
		zU16							muStride;			//!< Stride between 2 vertex entry
		zU8								muElementStart;		//!< First entry index in stream definition
		zU8								muElementCount;		//!< Number of different vertex element contained in buffer (not number of vertice)
	};

	virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zArrayStatic<zU8>					maElementDef;		//!< Vertex element definitions
	zcExp::StaticArraySerial<Stream>	maStream;			//!< Vertex Buffer data
	zFlagResUse							mResourceUse;		//!< Creation flags taken from (SerialVertex_Base::eCreationFlags)
};	

}

#endif
