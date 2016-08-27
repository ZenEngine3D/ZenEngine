#pragma once
#ifndef __zCore_Exp_Gfx_MeshStrip_Base_h__
#define __zCore_Exp_Gfx_MeshStrip_Base_h__

namespace zcExp
{
	struct ExportInfoGfxMeshStrip : public ExportInfoBase
	{
		zResID									mVertexBufferID;
		zResID									mIndexBufferID;
		zResID									mShaderBindingID;			
		zArrayStatic<zenRes::zShaderTexture>	maTexture;				//!< List of textures assigned to shader
		zArrayStatic<zResID>					maShaderParamID;		//!< List of Shader Param Instance (1 per ParamDef entry)
		zU32									muVertexFirst;
		zU32									muIndexFirst;
		zU32									muIndexCount;
	};
	
	class ExportGfxMeshStrip : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxMeshStrip, zenRes::zExportData)
	public:
		//virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zResID								mIndexBufferID;
		zResID								mStreamBindingID;
		zResID								mShaderBindingID;
		zArrayStatic<zResID>				maShaderParamID;		//!< Array of all ShaderParam used by all Shaders stage
		zArrayStatic<zArrayStatic<zResID>>	maTextureID;			//!< Per Shader stage texture input for each slot
		zArrayStatic<zArrayStatic<zResID>>	maSamplerID;			//!< Per Shader stage sampler state for each slot		
		zU32								muVertexFirst;
		zU32								muIndexFirst;
		zU32								muIndexCount;
	};

	class ExporterGfxMeshStrip : public ExporterBase
	{
	zenClassDeclare(ExporterGfxMeshStrip, ExporterBase)
	public:				
		typedef zEngineRef<ExportGfxMeshStrip>	ExportResultRef; 
												ExporterGfxMeshStrip(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool							ExportStart();
		ExportResultRef							mrExport;
	};

	zResID CreateGfxMeshStrip(zResID _VertexBufferID, zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0, const zArrayBase<zResID>& _aShaderParamID=zArrayStatic<zResID>(), const zArrayBase<zenRes::zShaderTexture>& _aTexture=zArrayStatic<zenRes::zShaderTexture>());
}

#endif