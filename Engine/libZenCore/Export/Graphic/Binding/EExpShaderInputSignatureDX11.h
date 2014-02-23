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
		//static awResourceID	ExtractID		( const Array<awU8>& _aShaderCode );

		awArrayStatic<awU8>				maDummyShaderCode;	

		//-------------------------------------------------------------------------------------------------
		// Export support section
		//-------------------------------------------------------------------------------------------------
	public:
		struct ExportInfoExt
		{				
			awArrayStatic<awU8>	maCompiledShader;
		};
		static awResourceID	CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:
		virtual bool				ExportStart();	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif