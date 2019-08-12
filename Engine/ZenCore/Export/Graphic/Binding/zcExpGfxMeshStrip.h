#pragma once

namespace zcExp
{
	struct ExportInfoGfxMeshStrip : public ExportInfoBase
	{
		zResID								mIndexBufferID;
		zResID								mShaderBindingID;			
		zArrayDyn<zenRes::zShaderResource>	maResources;			//!< List of buffer/textures/sampler assigned to shader
		zU32								muVertexFirst;
		zU32								muIndexFirst;
		zU32								muIndexCount;
	};
	
	class ExportGfxMeshStrip : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxMeshStrip, zenRes::zExportData)
	public:
		//virtual bool						Serialize( zcExp::Serializer_Base& _Serializer ){return true;}
		zResID								mIndexBufferID;
		zResID								mShaderBindingID;
		zArrayDyn<zResID>					maConstanBufferID;										//!< Array of all Constant Buffers used by all shader stages (also in maResourceID, but without double between various shader stages)
		zArrayDyn<zResID>					maResourceID[keShaderStage__Count][keShaderRes__Count];	//!< Shader per stage/resource slot input
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
		
	zResID CreateGfxMeshStrip(zResID _IndexBufferID, zResID _ShaderBindingID, zU32 _uIndexFirst=0, zU32 _uIndexCount=0xFFFFFFFF, zU32 _uVertexFirst = 0, /*const zArray<zResID>& _aShaderParamID=zArrayDyn<zResID>(),*/ const zArray<zenRes::zShaderResource>& _aResources=zArrayDyn<zenRes::zShaderResource>());
}
