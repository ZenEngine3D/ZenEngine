#pragma once
#if !defined(__LibExport_Graphics_SerialGfxVertex_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxVertex_DX11_h__

namespace EExp
{	

class SerialGfxVertex_DX11 : public SerialVertex_Base
{
AWClassDeclare(SerialGfxVertex_DX11, SerialVertex_Base)
//-------------------------------------------------------------------------------------------------
// Serialization and Asset creation support
//-------------------------------------------------------------------------------------------------
public:

	class Stream : public EExp::ISerialize
	{
	AWClassDeclare(Stream, EExp::ISerialize)
	public:		
		virtual bool					Serialize( EExp::Serializer_Base& _Serializer );		
		zenArrayStatic<zenU8>				maData;				//!< Vertex elements data (pos, uv, etc...)
		zenU16							muStride;			//!< Stride between 2 vertex entry
		zenU8							muElementStart;		//!< First entry index in stream definition
		zenU8							muElementCount;		//!< Number of different vertex element contained in buffer (not number of vertice)
	};
	
	struct SerialUseOnly
	{		
		zenArrayStatic<zenU8>				maElementDef;		//!< Vertex element definitions
		EExp::StaticArraySerial<Stream>	maStream;			//!< Vertex Buffer data
		zenU32							muCreationFlags;	//!< Creation flags taken from (SerialVertex_Base::eCreationFlags)
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