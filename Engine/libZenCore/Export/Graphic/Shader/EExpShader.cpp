#include "libZenCore.h"

namespace EExp
{
	zResID SerialShader_Base::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert( zenConst::kFlagResShaders.Any(_eType) );
		ZENAssert( _pExportInfo );
		const ExportInfo* pExportInfo		= static_cast<const ExportInfo*>(_pExportInfo);
		const zenRes::zShaderDefine* pEntry	= pExportInfo->maDefines.First();

		zResID::NameHash hName;
		hName.Append( pExportInfo->mzFilename );
		hName.Append( pExportInfo->mzEntryname );
		for(zUInt idx(0), count(pExportInfo->maDefines.Count()); idx<count; ++idx, ++pEntry)
		{
			hName.Append(pEntry->mzName);
			hName.Append(pEntry->mzValue);
		}

		return EExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
	}

	static zArrayStatic<zenRes::zShaderDefine> saEmptyDefines(zUInt(0));
	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname  )
	{
		return CreateGfxShaderVertex( _zFilename, _zEntryname, saEmptyDefines );		
	}
	zResID CreateGfxShaderPixel( const char* _zFilename, const char* _zEntryname )
	{
		return CreateGfxShaderPixel( _zFilename, _zEntryname, saEmptyDefines );
	}

	//=================================================================================================
	//! @brief		Create a new Vertex Shader
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param _zFilename	- Filename with code content
	//! @param _zEntryname	- Name of entry function
	//! @return 			- Resource created
	//=================================================================================================
	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines  )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderVertex", sizeof(SerialShader_Base::ExportInfo), 1, 5 );
		SerialShader_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShader_Base::ExportInfo;
		pExportInfo->mzFilename						= _zFilename;
		pExportInfo->mzEntryname					= _zEntryname;
		pExportInfo->maDefines						= _aDefines;
		pExportInfo->meShaderStage					= EExp::keShaderStage_Vertex;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderVertex, pExportInfo );
	}

	//=================================================================================================
	//! @brief		Create a new Pixel Shader
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param _zFilename	- Filename with code content
	//! @param _zEntryname	- Name of entry function
	//! @return 			- Resource created
	//=================================================================================================
	zResID CreateGfxShaderPixel( const char* _zFilename, const char* _zEntryname, const zArrayBase<zenRes::zShaderDefine>& _aDefines )
	{
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderPixel", sizeof(SerialShader_Base::ExportInfo), 1, 5 );
		SerialShader_Base::ExportInfo* pExportInfo	= zenNew(&sMemPool) SerialShader_Base::ExportInfo;
		pExportInfo->mzFilename						= _zFilename;
		pExportInfo->mzEntryname					= _zEntryname;
		pExportInfo->maDefines						= _aDefines;
		pExportInfo->meShaderStage					= EExp::keShaderStage_Pixel;
		return EMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderPixel, pExportInfo );
	}

	
}