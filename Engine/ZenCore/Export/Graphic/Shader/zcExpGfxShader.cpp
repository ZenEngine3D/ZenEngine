#include "zcCore.h"

namespace zcExp
{
	zResID ExportInfoGfxShader::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
	{
		ZENAssert( zenConst::kFlagResShaders.Any(_eType) );
		ZENAssert( _pExportInfo );
		const ExportInfoGfxShader* pExportInfo	= static_cast<const ExportInfoGfxShader*>(_pExportInfo);
		const zenRes::zShaderDefine* pEntry			= pExportInfo->maDefines.First();

		zResID::NameHash hName;
		hName.Append((const char*)pExportInfo->mzFilename );
		hName.Append((const char*)pExportInfo->mzEntryname );
		for(zUInt idx(0), count(pExportInfo->maDefines.Count()); idx<count; ++idx, ++pEntry)
		{
			hName.Append((const char*)pEntry->mzName);
			hName.Append((const char*)pEntry->mzValue);
		}

		return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
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
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderVertex", sizeof(ExportInfoGfxShader), 1, 5 );
		ExportInfoGfxShader* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxShader;
		pExportInfo->mzFilename				= _zFilename;
		pExportInfo->mzEntryname			= _zEntryname;
		pExportInfo->maDefines				= _aDefines;
		pExportInfo->meShaderStage			= zenConst::keShaderStage_Vertex;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderVertex, pExportInfo );
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
		static zenMem::zAllocatorPool sMemPool("Pool CreateShaderPixel", sizeof(ExportInfoGfxShader), 1, 5 );
		ExportInfoGfxShader* pExportInfo	= zenNew(&sMemPool) ExportInfoGfxShader;
		pExportInfo->mzFilename				= _zFilename;
		pExportInfo->mzEntryname			= _zEntryname;
		pExportInfo->maDefines				= _aDefines;
		pExportInfo->meShaderStage			= zenConst::keShaderStage_Pixel;
		return zcMgr::Export.CreateItem( zResID::kePlatformType_GFX, zenConst::keResType_GfxShaderPixel, pExportInfo );
	}

	
}