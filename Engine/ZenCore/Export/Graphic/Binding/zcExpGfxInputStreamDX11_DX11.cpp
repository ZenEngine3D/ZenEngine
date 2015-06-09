#include "zcCore.h"

namespace zcExp
{

zResID ExporterGfxInputStreamDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxInputStream);
	ZENAssert( _pExportInfo );
	const ExportInfoGfxInputStream* pExportInfo = static_cast<const ExportInfoGfxInputStream*>(_pExportInfo);

	zResID::NameHash hName = pExportInfo->mVertexBufferID.GetName();	//! @todo Optim: Different buffer with same format should return same id, saving a lot of binding
	hName.Append( &pExportInfo->mShaderInputSignatureID.GetName(), sizeof(zResID::NameHash) );
	return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
}

ExporterGfxInputStreamDX11_DX11::ExporterGfxInputStreamDX11_DX11(const ResDataRef& _rResData)
: ExporterBase(_rResData.GetSafe())
, mrResData(_rResData)
{
}

//=================================================================================================
//! @brief		Create a Vertex Input Signature
//! @details	Creates a dummy empty shader with only the input declared, so any vertex shader
//!				having the same signature, can use the same input layout object, and not keep 
//!				shader code around
//! @note		Can be called from Thread:Main or Thread:Task
//-------------------------------------------------------------------------------------------------
//! @return		True if successful
//=================================================================================================
bool ExporterGfxInputStreamDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	bool bSuccess(FALSE);
	ExportInfoGfxInputStream* pExportInfo	= static_cast<ExportInfoGfxInputStream*>(mpExportInfo);				
	mrResData->mVertexBufferID			= pExportInfo->mVertexBufferID;
	mrResData->mShaderInputSignatureID	= pExportInfo->mShaderInputSignatureID;
	return true;
}

}
