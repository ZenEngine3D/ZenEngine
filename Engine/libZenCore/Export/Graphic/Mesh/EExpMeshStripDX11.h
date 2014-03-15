#pragma once
#if !defined(__LibExport_Graphics_SerialGfxMeshStrip_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxMeshStrip_DX11_h__

namespace EExp
{	
	class SerialGfxMeshStrip_DX11 : public SerialMeshStrip_Base
	{
	ZENClassDeclare(SerialGfxMeshStrip_DX11, SerialMeshStrip_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool					Serialize( EExp::Serializer_Base& _Serializer );
		struct SerialUseOnly
		{
			zResID								mIndexBufferID;
			zResID								mStreamBindingID;
			zResID								mShaderBindingID;
			zArrayStatic<zResID>					maShaderParamID;		//!< Array of all ShaderParam used by all Shaders stage
			zArrayStatic<zArrayStatic<zResID>>	maTextureID;			//!< Per Shader stage texture input for each slot
			zArrayStatic<zArrayStatic<zResID>>	maSamplerID;			//!< Per Shader stage sampler state for each slot			
		};
		SerialUseOnly					mSerial;
		zU32								muIndexFirst;
		zU32								muIndexCount;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:			
		virtual bool					ExportWork(bool _bIsTHRTask);	
		virtual bool					ExportEnd();
	};

}

#endif