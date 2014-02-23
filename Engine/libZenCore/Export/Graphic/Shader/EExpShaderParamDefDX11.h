#pragma once
#if !defined(__LibExport_Graphics_ShaderParam_Info_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_ShaderParam_Info_DX11_h__

// Forward delcare
struct ID3D11ShaderReflectionConstantBuffer;

namespace EExp
{
	class SerialGfxShaderParamDef_DX11 : public SerialShaderParamDef_Base
	{
	AWClassDeclare(SerialGfxShaderParamDef_DX11, SerialShaderParamDef_Base)
	public:		
		//-------------------------------------------------------------------------------------------------
		// Serialization and Asset creation support
		//-------------------------------------------------------------------------------------------------
	public:		
		virtual bool				Serialize		( EExp::Serializer_Base& _Serializer );		

		eShaderParamFreq			meFrequence;		
		awArrayStatic<awU8>			maParameterDefaults;

		struct ExportInfoExt
		{
			awArrayStatic<awU8>		maCompiledShader;
		};
		static awResourceID 		CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	protected:	
		virtual bool				ExportStart();
		virtual bool				ExportWork(bool _bIsTHRTask);	
	};
}

#endif
