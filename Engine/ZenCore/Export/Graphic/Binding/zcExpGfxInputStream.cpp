#include "zcCore.h"

namespace zcExp
{
	zResID ExporterGfxInputStream::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		zenAssert(_eType==zenConst::keResType_GfxInputStream);
		zenAssert( _pExportInfo );
		const ExportInfoGfxInputStream* pExportInfo = static_cast<const ExportInfoGfxInputStream*>(_pExportInfo);

		zResID::NameHash hName = pExportInfo->mVertexBufferID.GetName();	//! @todo Optim: Different buffer with same format should return same id, saving a lot of binding
		hName.Append( &pExportInfo->mShaderInputSignatureID.GetName(), sizeof(zResID::NameHash) );
		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	ExporterGfxInputStream::ExporterGfxInputStream(const ExportResultRef& _rExportOut)
	: ExporterBase(_rExportOut.GetSafe())
	, mrExport(_rExportOut)
	{
		zenAssert(mrExport.IsValid());
	}

	//=================================================================================================
	//! @brief		Create a Vertex Input Signature
	//! @details	Creates a dummy empty shader with only the input declared, so any vertex shader
	//!				having the same signature, can use the same input layout object, and not keep 
	//!				shader code around
	//-------------------------------------------------------------------------------------------------
	//! @return		True if successful
	//=================================================================================================
	bool ExporterGfxInputStream::ExportStart()
	{
		if( !Super::ExportStart() )
			return false;
		
		ExportInfoGfxInputStream* pExportInfo	= static_cast<ExportInfoGfxInputStream*>(mpExportInfo);				
		mrExport->mVertexBufferID				= pExportInfo->mVertexBufferID;
		mrExport->mShaderInputSignatureID		= pExportInfo->mShaderInputSignatureID;
		ExportSkipWork();
		return true;
	}

	//=================================================================================================
	//! @brief		Create a new ShaderInputSignature Binding to a VertexBuffer
	//! @details	Tie together a vertex stream and input signature, for rendering
	//-------------------------------------------------------------------------------------------------
	//! @param _VertexBufferID			- VertexBufer ID to bind with ShaderInputSignature
	//! @param _ShaderInputSignatureID	- ShaderInputSignature ID to bind with a particular VertexBuffer
	//! @return 						- Unique zResID of created Resource
	//=================================================================================================
	zResID CreateGfxInputStream(zResID _VertexBufferID, zResID _ShaderInputSignatureID)
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateInputStream", sizeof(ExportInfoGfxInputStream), 1, 5 );
		ExportInfoGfxInputStream* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxInputStream;
		pExportInfo->mVertexBufferID			= _VertexBufferID;
		pExportInfo->mShaderInputSignatureID	= _ShaderInputSignatureID;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxInputStream, pExportInfo );
	}

}

