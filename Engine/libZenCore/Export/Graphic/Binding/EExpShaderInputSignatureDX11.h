#pragma once
#if !defined(__LibExport_Graphics_GfxShaderInputSignature_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxShaderInputSignature_DX11_h__

namespace EExp
{	
	class SerialGfxInputSignature_DX11 : public SerialInputSignature_Base
	{
	AWClassDeclare(SerialGfxInputSignature_DX11, SerialInputSignature_Base)
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( EExp::Serializer_Base& _Serializer );
		//static zenResID	ExtractID		( const Array<zenU8>& _aShaderCode );

		zenArrayStatic<zenU8>				maDummyShaderCode;	

		//-------------------------------------------------------------------------------------------------
		// Export support section
		//-------------------------------------------------------------------------------------------------
	public:
		struct ExportInfoExt
		{				
			zenArrayStatic<zenU8>	maCompiledShader;
		};
		static zenResID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:
		virtual bool				ExportStart();	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif