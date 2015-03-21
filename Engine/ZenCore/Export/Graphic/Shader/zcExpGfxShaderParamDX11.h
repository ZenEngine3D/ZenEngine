#pragma once
#if !defined(__zCore_Exp_Gfx_GfxShaderParam_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_GfxShaderParam_DX11_h__

namespace zcExp
{

class SerialGfxShaderParam_DX11 : public SerialShaderParam_Base
{
ZENClassDeclare(SerialGfxShaderParam_DX11, SerialShaderParam_Base)
//-------------------------------------------------------------------------------------------------	
// Serialization and Asset creation support	
//-------------------------------------------------------------------------------------------------
public:
	virtual bool			Serialize( zcExp::Serializer_Base& _Serializer );
	zArrayStatic<zU8>			maParameterValues;			//!< Bytes data for all buffer values (see ShaderParamDef for each parameter start offset in array)
		
//-------------------------------------------------------------------------------------------------
// Export support section
//-------------------------------------------------------------------------------------------------
protected:	
	virtual bool			ExportWork(bool _bIsTHRTask);	
	virtual bool			ExportEnd();
};
}

#endif
