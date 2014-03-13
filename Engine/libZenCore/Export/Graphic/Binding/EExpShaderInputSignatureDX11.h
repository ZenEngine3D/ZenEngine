#pragma once
#if !defined(__LibExport_Graphics_GfxShaderInputSignature_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxShaderInputSignature_DX11_h__

namespace EExp
{	
	class SerialGfxInputSignature_DX11 : public SerialInputSignature_Base
	{
	ZENClassDeclare(SerialGfxInputSignature_DX11, SerialInputSignature_Base)
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( EExp::Serializer_Base& _Serializer );
		//static zResID	ExtractID		( const Array<zU8>& _aShaderCode );

		zArrayStatic<zU8>				maDummyShaderCode;	

		//-------------------------------------------------------------------------------------------------
		// Export support section
		//-------------------------------------------------------------------------------------------------
	public:
		struct ExportInfoExt
		{				
			zArrayStatic<zU8>	maCompiledShader;
		};
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:
		virtual bool				ExportStart();	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif