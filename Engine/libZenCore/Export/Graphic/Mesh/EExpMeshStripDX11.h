#pragma once
#if !defined(__LibExport_Graphics_SerialGfxMeshStrip_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_SerialGfxMeshStrip_DX11_h__

namespace EExp
{	
	class SerialGfxMeshStrip_DX11 : public SerialMeshStrip_Base
	{
	AWClassDeclare(SerialGfxMeshStrip_DX11, SerialMeshStrip_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool					Serialize( EExp::Serializer_Base& _Serializer );
		struct SerialUseOnly
		{
			awResourceID								mIndexBufferID;
			awResourceID								mStreamBindingID;
			awResourceID								mShaderBindingID;
			awArrayStatic<awResourceID>					maShaderParamID;		//!< Array of all ShaderParam used by all Shaders stage
			awArrayStatic<awArrayStatic<awResourceID>>	maTextureID;			//!< Per Shader stage texture input for each slot
			awArrayStatic<awArrayStatic<awResourceID>>	maSamplerID;			//!< Per Shader stage sampler state for each slot			
		};
		SerialUseOnly					mSerial;
		awU32								muIndexFirst;
		awU32								muIndexCount;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:			
		virtual bool					ExportWork(bool _bIsTHRTask);	
		virtual bool					ExportEnd();
	};

}

#endif