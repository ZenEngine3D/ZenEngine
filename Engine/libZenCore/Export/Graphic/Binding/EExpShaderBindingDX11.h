#pragma once
#if !defined(__LibExport_Graphics_GfxShaderBinding_DX11_h__) && AW_EXPORT_OR_RESOURCE_DX11
#define __LibExport_Graphics_GfxShaderBinding_DX11_h__

namespace EExp
{	
	class SerialGfxShaderBinding_DX11 : public SerialShaderBinding_Base
	{
	AWClassDeclare(SerialGfxShaderBinding_DX11, SerialShaderBinding_Base)
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
				AWStaticAssertMsg( sizeof(zenU64) >= sizeof(zenU8)*EExp::keShaderStage__Count, "More stage that can fit in a zenU64" );
			}

			virtual bool Serialize( EExp::Serializer_Base& _Serializer )
			{
				//! @todo serialize
				return true;
			}
			 
			union { zenU8 muSlot[EExp::keShaderStage__Count]; zenU64 muCollapsedSlots; };
			union {	zenU8 muCount[EExp::keShaderStage__Count]; zenU64 muCollapsedCounts; };
		};

		virtual bool						Serialize				( EExp::Serializer_Base& _Serializer );
	
		struct SerialUseOnly
		{
			zenArrayStatic<zenResID>	maShaderID;			//!< Shader used in each shader stage
			zenArrayStatic<zenResID>	maParamDefID;		//!< Shader parameter def used by all bound shader stage
		};
		SerialUseOnly					mSerial;			//!< Only use for exporting/loading resource
		zenArrayStatic<zenU32>			maStagePerParamDef;	//!< Array of Bitfield of valid shader stage per ShaderParam (match mSerial.maParamDef)
		zenArrayStatic<zenHash32>			maTextureName;		//!< List of all texture names used in this binding
		zenArrayStatic<TextureSlot>		maTextureBind;		//!< Info on texture slots used (1 per TextureName)
		zenArrayStatic<zenHash32>			maParameterName;	//!< List of all parameter names used in this binding
		zenArrayStatic<zenU32>			maParameterMask;	//!< List of ShaderParam parameter are used in (1 entry per maParameterName)		

	//-------------------------------------------------------------------------------------------------
	// Export support section
	//-------------------------------------------------------------------------------------------------
	protected:	
		virtual bool						ExportStart();
	};

}

#endif