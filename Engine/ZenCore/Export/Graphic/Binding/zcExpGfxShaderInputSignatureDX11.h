#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShaderInputSignature_DX11_h__

namespace zcExp
{	
	class SerialGfxInputSignature_DX11 : public SerialInputSignature_Base
	{
	ZENClassDeclare(SerialGfxInputSignature_DX11, SerialInputSignature_Base)
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( zcExp::Serializer_Base& _Serializer );
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
		static zResID	CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:
		virtual bool				ExportStart();	
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};

}

#endif