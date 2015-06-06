#pragma once
#ifndef __zCore_Exp_Gfx_ShaderBinding_Base_h__
#define __zCore_Exp_Gfx_ShaderBinding_Base_h__

namespace zcExp
{
	struct ExportInfoGfxShaderBinding : public ExportInfoBase
	{
		zArrayStatic<zResID>	maShaderID;			//!< List of shaders to tie together (vertex, pixel, ...)			
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	class ResDataGfxShaderBinding : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxShaderBinding, ResDataBase)
	public:
		//! @class Used to track texture SlotIndex and SlotCount
		struct TextureSlot : public zcExp::ISerialize
		{	
			TextureSlot() 
			: muCollapsedSlots(0)
			, muCollapsedCounts(0)
			{
				ZENStaticAssertMsg( sizeof(zU64) >= sizeof(zU8)*zenConst::keShaderStage__Count, "More stage that can fit in a zU64" );
			}

			virtual bool Serialize( zcExp::Serializer_Base& _Serializer ){ return true; } //! @todo Missing: serialize			 
			union { zU8 muSlot[zenConst::keShaderStage__Count]; zU64 muCollapsedSlots; };
			union {	zU8 muCount[zenConst::keShaderStage__Count]; zU64 muCollapsedCounts; };
		};

		virtual bool				Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zArrayStatic<zResID>		maShaderID;			//!< Shader used in each shader stage
		zArrayStatic<zResID>		maParamDefID;		//!< Shader parameter definition used by all bound shader stages				//! @todo clean export save/load dictionary instead of 2 arrays
		zArrayStatic<zU32>			maStagePerParamDef;	//!< Array of Bitfield of valid shader stage per ShaderParam (match maParamDefID) 
		zArrayStatic<zHash32>		maTextureName;		//!< List of all texture names used in this binding								//! @todo clean export save/load dictionary instead of 2 arrays
		zArrayStatic<TextureSlot>	maTextureBind;		//!< Info on texture slots used (1 per TextureName)
		zArrayStatic<zHash32>		maParameterName;	//!< List of all parameter names used in this binding							//! @todo clean export save/load dictionary instead of 2 arrays
		zArrayStatic<zU32>			maParameterMask;	//!< List of ShaderParam parameter are used in (1 entry per maParameterName)
	};

	class ExporterGfxShaderBinding : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxShaderBinding, ExporterBase)
	public:											
											ExporterGfxShaderBinding(const ResDataRef& _rResData);
	protected:	
		virtual bool						ExportStart();		
		zMap<zU32>::Key64					mdStagePerParamDef;		//!< ParamDef needed for this binding, with stage mask they applied to (automatically computed at export)
	};

	zResID CreateGfxShaderBinding(const zArrayBase<zResID>& _aShaderID);	
}

#endif
