#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxMeshStrip_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxMeshStrip_DX11_h__

namespace zcExp
{	
	class SerialGfxMeshStrip_DX11 : public SerialMeshStrip_Base
	{
	ZENClassDeclare(SerialGfxMeshStrip_DX11, SerialMeshStrip_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:
		virtual bool					Serialize( zcExp::Serializer_Base& _Serializer );
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