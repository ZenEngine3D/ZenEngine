#pragma once

namespace zcExp
{
	struct ExportInfoGfxShader : public ExportInfoBase
	{				
		static zResID						CallbackGetItemID(ePlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
		zString								mzFilename;
		zString								mzEntryname;
		zArrayDyn<zenRes::zShaderDefine>	maDefines;
		eShaderStage						meShaderStage;
	};

	class ExportGfxShader : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxShader, zenRes::zExportData)
	public:
		//virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return FALSE;}
		struct ShaderBindInfo{ 
			zStringHash32	mzName;												//!< Name given to resource with hash
			zU32			muMemSize;											//!< Size taken by one entry of StructBuffer or entire ConstantBuffer (if applicable)
			eShaderResource	meType;												//!< Resource type
			zU8				muSlotIndex;										//!< Resource bind point //! @todo 3 change this to u16
			zU8				muSlotCount;										//!< Resource array entries count
		};
																				//!< @todo feature support buffer slots
		eShaderStage				meShaderStage;								//!< Type of shader (vertex, pixel, ...)
		zArrayDyn<zResID>			maCBufferParentID;							//!< List of CBufferDefinition used by this shader
		zArrayDyn<zU8>				maCBufferResIndex;							//!< Index in maResourceBinding for associated CBufferDefinition resource
		zArrayDyn<ShaderBindInfo>	maResourceBinding;							//!< List of shader resources infos used
		zU32						maResourceBindCount[keShaderRes__Count];	//!< Number of binding point for each resource type (ex: array of 3 textures has 1 BindCount but 3 ResourceCount)
		zU32						maResourceBindMax[keShaderRes__Count];		//!< Maximum input slot index used by each resource type
	};

	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname );
	zResID CreateGfxShaderVertex( const char* _zFilename, const char* _zEntryname, const zArray<zenRes::zShaderDefine>& _aDefines );
	zResID CreateGfxShaderPixel	( const char* _zFilename, const char* _zEntryname, const zArray<zenRes::zShaderDefine>& _aDefines );
}
