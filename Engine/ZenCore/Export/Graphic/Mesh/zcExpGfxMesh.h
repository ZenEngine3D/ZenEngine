#pragma once
#ifndef __zCore_Exp_Gfx_Mesh_Base_h__
#define __zCore_Exp_Gfx_Mesh_Base_h__

namespace zcExp
{	
	struct ExportInfoGfxMesh : public ExportInfoBase
	{
		zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};

	class ExportGfxMesh : public zenRes::zExportData
	{
	zenClassDeclare(ExportGfxMesh, zenRes::zExportData)
	public:
		//virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zArrayStatic<zResID>	maMeshStripID;			//!< List of Mesh strips used this Mesh
	};

	class ExporterGfxMesh : public ExporterBase
	{
	zenClassDeclare(ExporterGfxMesh, ExporterBase)
	public:				
	typedef zEngineRef<ExportGfxMesh>	ExportResultRef; 
										ExporterGfxMesh(const ExportResultRef& _rExportOut);
	protected:	
		virtual bool					ExportStart();		
		ExportResultRef					mrExport;
	};

	zResID CreateGfxMesh(const zArrayBase<zResID>& _aMeshStripID);
}

#endif