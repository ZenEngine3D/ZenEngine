#pragma once

namespace zcExp
{
	struct ExportInfoGfxShaderBinding : public ExportInfoBase
	{
		zArrayDyn<zResID>		maShaderID;			//!< List of shaders to tie together (vertex, pixel, ...)			
		static zResID			CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	//! @todo clean will need a common object for shaderbinsing instance, shared between computer shader and Meshstrip
	class ExportGfxShaderBinding : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxShaderBinding, zenRes::zExportData)
	public:
		union ShaderBindInfoIndex
		{
			zU8	 muShaderResIndex[keShaderStage__Count];				//!< Index in each shader stage maResourceBinding
			zU64 muResourceCollapsed=0xFFFFFFFFFFFFFFFF;
		};

		//virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zArrayDyn<zResID>					maShaderID;					//!< ShaderID of each shader stage
		zArrayDyn<zResID>					maCBufferParentID;			//!< List of CBufferDef ResID used in any shader stages of thsi binding
		zArrayDyn<zU8>						maCBufferParentBindIndex;	//!< List of CBufferDef index in this binding list of resources (match CBufferParentID)
		zArrayDyn<zHash32>					maCBufferParamName;			//!< List of parameter names used in any shaders stages of thsi binding
		zArrayDyn<zU32>						maCBufferParamMask;			//!< List of CBufferDef this parameter name is used in	(match maCBufferParamName)
		zArrayDyn<zHash32>					maResourceName;				//!< List of resources names used in this binding
		zArrayDyn<ShaderBindInfoIndex>		maResourceBind;				//!< List of resources binding info in each shader stage (match maResourceName index)
		
		//! @todo Clean (should go in common resource class)		
		zMap<ShaderBindInfoIndex>::Key32	mdResourceBind;				//!< Hashmap of Resources binding info per resource name
		zMap<zU32>::Key32					mdCBufferParamMask;			//!< Hashmap of which CBufferDefinition a parameter name is in
	};

	class ExporterGfxShaderBinding : public ExporterBase
	{
	zenClassDeclare(ExporterGfxShaderBinding, ExporterBase)
	public:						
	typedef zEngineRef<ExportGfxShaderBinding>	ExportResultRef; //todo use proper ref class
												ExporterGfxShaderBinding(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool							ExportStart();		
		ExportResultRef							mrExport;
	};

	zResID CreateGfxShaderBinding(const zArray<zResID>& _aShaderID);	
}
