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
			zArrayStatic<zResID>	maTargetColorID;
			zResID				mTargetDepthID;
			zVec2U16						mvOrigin;
			zVec2U16						mvDim;
		};
		struct SerialUseOnly
		{							
			zArrayStatic<zResID>	maTargetColorID;
			zResID				mTargetDepthID;
		};
		virtual bool						ExportStart();
		static zResID			CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
		
		SerialUseOnly						mSerial;
		zVec2U16							mvOrigin;
		zVec2U16							mvDim;
	};

	zResID CreateGfxView( const zResID& _TargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2U16& _vOrigin=zVec2U16(0,0) );
	zResID CreateGfxView( const zArrayBase<zResID>& _aTargetColorID, const zResID& _TargetDepthID, const zVec2U16& _vDim=zVec2U16(9999,9999), const zVec2U16& _vOrigin=zVec2U16(0,0) );
}


#endif