#pragma once

//! @todo clean rename this file
namespace zcExp
{
	//! @todo clean simplify this struct, might not need all this details
	struct GfxCBufferParamInfo
	{
		zU32	muOffset		: 16;	//!< Offset of value in constant buffer or register index (maximum 64k offset)
		zU32	muSize			: 13;	//!< Parameter entire memory size (maximum 8192 bytes)
		//zU32	muColCount		: 4;	//!< Number of element in vector (1 if scalar)
		zU32	muRowCount		: 4;	//!< Number of lines in matrix (1 if scalar or vector)
		zU32	muArrayCount	: 10;	//!< Number of entries in array (support maximum 1024, 0 if not an array)
		zU32	muArrayItemSize	: 10;	//!< Size of 1 array entry including padding (maximum 1024 bytes)
		zU32	meType			: 8;	//!< eShaderElementType entry (float,int,...)
		zU32	mbInUse			: 1;	//!< Shader parameter used in shader or not	
	};

	struct ExportInfoGfxCBufferDefinition : public ExportInfoBase
	{
		zStringHash32						mzBufferName;
		zArrayStatic<zHash32>				maParamName;
		zArrayStatic<GfxCBufferParamInfo>	maParamInfo;
		zArrayStatic<zU8,16>				maDefaultValues;
		static zResID 						CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	class ExportGfxCBufferDefinition : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxCBufferDefinition, zenRes::zExportData)
	public:
		zStringHash32						mzBufferName;
		zMap<GfxCBufferParamInfo>::Key32	mdParamInfo;		//!< Dictionary of Constant Buffer's parameter informations, by name hash
		zArrayStatic<zU8,16>				muDefaultValues;	//!< CBuffer default value when crated from this definition (extracted from shader code)
	};

	class ExporterGfxShaderParamDef : public ExporterBase
	{
	zenClassDeclare(ExporterGfxShaderParamDef, ExporterBase)
	public:	
	typedef zEngineRef<ExportGfxCBufferDefinition>	ExportResultRef; //todo use proper ref class
													ExporterGfxShaderParamDef(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool								ExportStart();
		ExportResultRef								mrExport;
	};

	zResID CreateGfxCBufferDefinition( const zStringHash32& _zBufferName, const zArrayBase<zHash32>& _aParamName, const zArrayBase<GfxCBufferParamInfo>& _aParamInfo, const zArrayBase<zU8>& _uDefaultValues );
}
