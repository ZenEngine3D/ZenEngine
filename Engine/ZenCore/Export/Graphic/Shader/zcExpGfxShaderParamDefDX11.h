#pragma once
#if !defined(__zCore_Exp_Gfx_ShaderParam_Info_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_ShaderParam_Info_DX11_h__

// Forward delcare
struct ID3D11ShaderReflectionConstantBuffer;

namespace zcExp
{
	class SerialGfxShaderParamDef_DX11 : public SerialShaderParamDef_Base
	{
	ZENClassDeclare(SerialGfxShaderParamDef_DX11, SerialShaderParamDef_Base)
	public:		
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( zcExp::Serializer_Base& _Serializer );		

		eShaderParamFreq			meFrequence;		
		zArrayStatic<zU8>			maParameterDefaults;

		struct ExportInfoExt
		{
			zArrayStatic<zU8>		maCompiledShader;
		};
		static zResID 		CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportStart();
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};
}

#endif
