#pragma once
#ifndef __zCore_Exp_Gfx_InputStreamBinding_Base_h__
#define __zCore_Exp_Gfx_InputStreamBinding_Base_h__

namespace zcExp
{

struct ExportInfoGfxInputStream : public ExportInfoBase
{			
	zResID	mVertexBufferID;
	zResID	mShaderInputSignatureID;
};

class ExportGfxInputStream : public zenRes::zExportData
{
zenClassDeclare(ExportGfxInputStream, zenRes::zExportData)
public:
	//virtual bool	Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
	zResID			mVertexBufferID;
	zResID			mShaderInputSignatureID;
};

class ExporterGfxInputStream : public ExporterBase
{
zenClassDeclare(ExporterGfxInputStream, ExporterBase)
public:				
typedef zEngineRef<ExportGfxInputStream>	ExportResultRef; //todo use proper ref class
											ExporterGfxInputStream(const ExportResultRef& _rExportOut);		
	static zResID							CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
protected:	
	virtual bool							ExportStart();
	ExportResultRef							mrExport;
};

zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID);

}

#endif