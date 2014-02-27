#pragma once
#if !defined(__LibExport_Graphics_SerialGfxMesh_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxMesh_DX11_h__

namespace EExp
{	
	class SerialGfxMesh_DX11 : public SerialMesh_Base
	{
	ZENClassDeclare(SerialGfxMesh_DX11, SerialMesh_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool						Serialize( EExp::Serializer_Base& _Serializer );
		struct SerialUseOnly
		{
			zenArrayStatic<zenResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
			//! @todo generate strip per vb for depth rendering
		};
		SerialUseOnly					mSerial;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:			
		virtual bool					ExportStart();
	};

}

#endif