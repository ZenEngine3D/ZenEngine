#pragma once
#if !defined(__LibExport_Graphics_GfxShader_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxShader_DX11_h__

namespace EExp
{	
	//! @todo Move ShaderParamDef here instead of ShaderBinding?
	class SerialShader_DX11 : public SerialShader_Base
	{
	ZENClassDeclare(SerialShader_DX11, SerialShader_Base)
	public:
		struct SerialUseOnly
		{
			zArrayStatic<zU8>	maCompiledShader;			
		};
		SerialUseOnly	mSerialCommon;
	protected:
		bool			ExportWorkCompile();
		bool			ExportWorkExtractResources();		
		virtual bool	ExportEnd();
	};

	class SerialGfxShaderPixel_DX11 : public SerialShader_DX11
	{
	ZENClassDeclare(SerialGfxShaderPixel_DX11, SerialShader_DX11)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize				( EExp::Serializer_Base& _Serializer );
	
	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);			
	};


	class SerialGfxShaderVertex_DX11 : public SerialShader_DX11
	{
	ZENClassDeclare(SerialGfxShaderVertex_DX11, SerialShader_DX11)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize				( EExp::Serializer_Base& _Serializer );

		struct SerialUseOnly
		{
			zResID			mShaderInputSignatureID;
		};
		SerialUseOnly				mSerial;

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool				ExportWork(bool _bIsTHRTask);	
		virtual	bool				ExportEnd();
	};

}

#endif