#pragma once
#ifndef __zCore_Exp_Gfx_View_Base_h__
#define __zCore_Exp_Gfx_View_Base_h__

namespace zcExp
{
	struct ExportInfoGfxView : public ExportInfoBase
	{
		zArrayStatic<zResID>	maTargetColorID;
		zResID					mTargetDepthID;
		zVec2S16				mvOrigin;
		zVec2U16				mvDim;
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const ExportInfoBase* _pExportInfo, bool& _bExistOut);
	};
	
	class ExportDataGfxView : public ExportDataBase
	{
	ZENClassDeclare(ExportDataGfxView, ExportDataBase)
	public:
		virtual bool			Serialize( zcExp::Serializer_Base& _Serializer ){return true;}		
		zArrayStatic<zResID>	maTargetColorID;
		zResID					mTargetDepthID;
		zVec2S16				mvOrigin;
		zVec2U16				mvDim;
	};

	class ExporterGfxView : public ExporterBase
	{
	ZENClassDeclare(ExporterGfxView, ExporterBase)
	public:				
	typedef ExportDataGfxView		ExportData;
	typedef zGameRef<ExportData>	ExportDataRef;
									ExporterGfxView(const ExportDataRef& _rExportData);
	protected:	
		virtual bool				ExportStart();		
		ExportDataRef				mrExportData;
	};

	zResID CreateGfxView( const zResID& _TargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
	zResID CreateGfxView( const zArrayBase<zResID>& _aTargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
}


#endif