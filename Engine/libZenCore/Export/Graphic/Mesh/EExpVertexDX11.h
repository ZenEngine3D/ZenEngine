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
		awArrayStatic<awU8>				maData;				//!< Vertex elements data (pos, uv, etc...)
		awU16							muStride;			//!< Stride between 2 vertex entry
		awU8							muElementStart;		//!< First entry index in stream definition
		awU8							muElementCount;		//!< Number of different vertex element contained in buffer (not number of vertice)
	};
	
	struct SerialUseOnly
	{		
		awArrayStatic<awU8>				maElementDef;		//!< Vertex element definitions
		EExp::StaticArraySerial<Stream>	maStream;			//!< Vertex Buffer data
		awU32							muCreationFlags;	//!< Creation flags taken from (SerialVertex_Base::eCreationFlags)
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