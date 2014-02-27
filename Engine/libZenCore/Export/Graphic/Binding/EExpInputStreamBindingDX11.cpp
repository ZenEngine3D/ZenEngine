#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

namespace EExp
{
	bool SerialGfxInputStream_DX11::Serialize( EExp::Serializer_Base& _Serializer )
	{
		return FALSE; 
	}

	zenResID SerialGfxInputStream_DX11::CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert(_ePlatform==awconst::keResPlatform_DX11 && _eType==awconst::keResType_GfxInputStream);
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo = static_cast<const ExportInfo*>(_pExportInfo);

		zenResID::NameHash hName = pExportInfo->mVertexBufferID.Name();	//! @todo: Different buffer with same format should return same id, saving a lot of binding
		hName.Append( pExportInfo->mShaderInputSignatureID.Name() );
		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	//=================================================================================================
	//! @brief		Create a Vertex Input Signature
	//! @details	Creates a dummy emppty shader with only the input declared, so any vertex shader
	//!				having the same signature, can use the same input layout object, and not keep 
	//!				shader code around
	//! @note		Can be called from Thread:Main or Thread:Task
	//-------------------------------------------------------------------------------------------------
	//! @return		True if successful
	//=================================================================================================
	bool SerialGfxInputStream_DX11::ExportWork(bool _bIsTHRTask)
	{
		bool bSuccess(FALSE);
		ExportInfo* pExportInfo			= static_cast<ExportInfo*>(mpExportInfo);				
		mSerial.mVertexBufferID			= pExportInfo->mVertexBufferID;
		mSerial.mShaderInputSignatureID	= pExportInfo->mShaderInputSignatureID;
		return true;
	}
}

#endif //AW_EXPORT_OR_RESOURCE_DX11
