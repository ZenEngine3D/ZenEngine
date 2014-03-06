#pragma once
#if !defined(__LibExport_Graphics_ShaderParam_Info_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_ShaderParam_Info_DX11_h__

// Forward delcare
struct ID3D11ShaderReflectionConstantBuffer;

namespace EExp
{
	class SerialGfxShaderParamDef_DX11 : public SerialShaderParamDef_Base
	{
	ZENClassDeclare(SerialGfxShaderParamDef_DX11, SerialShaderParamDef_Base)
	public:		
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( EExp::Serializer_Base& _Serializer );		

		eShaderParamFreq			meFrequence;		
		zenArrayStatic<zenU8>			maParameterDefaults;

		struct ExportInfoExt
		{
			zenArrayStatic<zenU8>		maCompiledShader;
		};
		static zenResID 		CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportStart();
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};
}

#endif
