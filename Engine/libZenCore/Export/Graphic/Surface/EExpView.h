#pragma once
#ifndef __LibExport_Graphics_View_Base_h__
#define __LibExport_Graphics_View_Base_h__

namespace EExp
{
	class SerialGfxView_Base : public EExp::ExportItem
	{
	ZENClassDeclare(SerialGfxView_Base, EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			zenArrayStatic<zenResID>	maTargetColorID;
			zenResID				mTargetDepthID;
			zenVec2U16						mvOrigin;
			zenVec2U16						mvDim;
		};
		struct SerialUseOnly
		{							
			zenArrayStatic<zenResID>	maTargetColorID;
			zenResID				mTargetDepthID;
		};
		virtual bool						ExportStart();
		static zenResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
		
		SerialUseOnly						mSerial;
		zenVec2U16							mvOrigin;
		zenVec2U16							mvDim;
	};

	zenResID CreateGfxView( const zenResID& _TargetColorID, const zenResID& _TargetDepthID, const zenVec2U16& _vDim=zenVec2U16(9999,9999), const zenVec2U16& _vOrigin=zenVec2U16(0,0) );
	zenResID CreateGfxView( const zenArrayBase<zenResID>& _aTargetColorID, const zenResID& _TargetDepthID, const zenVec2U16& _vDim=zenVec2U16(9999,9999), const zenVec2U16& _vOrigin=zenVec2U16(0,0) );
}


#endif