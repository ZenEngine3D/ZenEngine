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
	
	class ResDataGfxView : public ResDataBase
	{
	ZENClassDeclare(ResDataGfxView, ResDataBase)
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
	typedef ResDataGfxView		ResData;
	typedef zGameRef<ResData>	ResDataRef;
									ExporterGfxView(const ResDataRef& _rResData);
	protected:	
		virtual bool				ExportStart();		
		ResDataRef				mrResData;
	};

	zResID CreateGfxView( const zResID& _TargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
	zResID CreateGfxView( const zArrayBase<zResID>& _aTargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2S16& _vOrigin=zVec2S16(0,0) );
}


#endif