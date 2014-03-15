#pragma once
#if !defined(__LibExport_Graphics_SerialGfxVertex_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxVertex_DX11_h__

namespace EExp
{	

class SerialGfxVertex_DX11 : public SerialVertex_Base
{
ZENClassDeclare(SerialGfxVertex_DX11, SerialVertex_Base)
//-------------------------------------------------------------------------------------------------
// Serialization and Asset creation support
//-------------------------------------------------------------------------------------------------
public:

	class Stream : public EExp::ISerialize
	{
	ZENClassDeclare(Stream, EExp::ISerialize)
	public:		
		virtual bool					Serialize( EExp::Serializer_Base& _Serializer );		
		zArrayStatic<zU8>				maData;				//!< Vertex elements data (pos, uv, etc...)
		zU16							muStride;			//!< Stride between 2 vertex entry
		zU8								muElementStart;		//!< First entry index in stream definition
		zU8								muElementCount;		//!< Number of different vertex element contained in buffer (not number of vertice)
	};
	
	struct SerialUseOnly
	{		
		zArrayStatic<zU8>				maElementDef;		//!< Vertex element definitions
		EExp::StaticArraySerial<Stream>	maStream;			//!< Vertex Buffer data
		zFlagResUse						mResourceUse;		//!< Creation flags taken from (SerialVertex_Base::eCreationFlags)
	};
	SerialUseOnly						mSerial;
	virtual bool						Serialize( EExp::Serializer_Base& _Serializer );		

//-------------------------------------------------------------------------------------------------
// Export support section
//-------------------------------------------------------------------------------------------------
protected:	
	virtual bool						ExportWork(bool _bIsTHRTask);	
};

}

#endif