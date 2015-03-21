#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxMesh_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxMesh_DX11_h__

namespace zcExp
{	
	class SerialGfxMesh_DX11 : public SerialMesh_Base
	{
	ZENClassDeclare(SerialGfxMesh_DX11, SerialMesh_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool						Serialize( zcExp::Serializer_Base& _Serializer );
		struct SerialUseOnly
		{
			zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
			//! @todo Missing: Generate strip per vb for depth rendering
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