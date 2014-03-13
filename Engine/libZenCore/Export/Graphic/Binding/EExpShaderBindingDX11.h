#pragma once
#if !defined(__LibExport_Graphics_GfxShaderBinding_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxShaderBinding_DX11_h__

namespace EExp
{	
	class SerialGfxShaderBinding_DX11 : public SerialShaderBinding_Base
	{
	ZENClassDeclare(SerialGfxShaderBinding_DX11, SerialShaderBinding_Base)
	//-------------------------------------------------------------------------------------------------
	// Serialization and Asset creation support
	//-------------------------------------------------------------------------------------------------
	public:		
		//! @class Used to track texture SlotIndex and SlotCount
		struct TextureSlot : public EExp::ISerialize
		{	
			TextureSlot() 
			: muCollapsedSlots(0)
			, muCollapsedCounts(0)
			{
				ZENStaticAssertMsg( sizeof(zU64) >= sizeof(zU8)*EExp::keShaderStage__Count, "More stage that can fit in a zU64" );
			}

			virtual bool Serialize( EExp::Serializer_Base& _Serializer )
			{
				//! @todo serialize
				return true;
			}
			 
			union { zU8 muSlot[EExp::keShaderStage__Count]; zU64 muCollapsedSlots; };
			union {	zU8 muCount[EExp::keShaderStage__Count]; zU64 muCollapsedCounts; };
		};

		virtual bool						Serialize				( EExp::Serializer_Base& _Serializer );
	
		struct SerialUseOnly
		{
			zArrayStatic<zResID>	maShaderID;			//!< Shader used in each shader stage
			zArrayStatic<zResID>	maParamDefID;		//!< Shader parameter def used by all bound shader stage
		};
		SerialUseOnly					mSerial;			//!< Only use for exporting/loading resource
		zArrayStatic<zU32>			maStagePerParamDef;	//!< Array of Bitfield of valid shader stage per ShaderParam (match mSerial.maParamDef)
		zArrayStatic<zHash32>			maTextureName;		//!< List of all texture names used in this binding
		zArrayStatic<TextureSlot>		maTextureBind;		//!< Info on texture slots used (1 per TextureName)
		zArrayStatic<zHash32>			maParameterName;	//!< List of all parameter names used in this binding
		zArrayStatic<zU32>			maParameterMask;	//!< List of ShaderParam parameter are used in (1 entry per maParameterName)		

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool						ExportStart();
	};

}

#endif