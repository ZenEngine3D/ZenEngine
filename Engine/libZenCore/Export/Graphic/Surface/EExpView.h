#pragma once
#ifndef __LibExport_Graphics_View_Base_h__
#define __LibExport_Graphics_View_Base_h__

namespace EExp
{
	class SerialGfxView_Base : public EExp::ExportItem
	{
	AWClassDeclare(SerialGfxView_Base, EExp::ExportItem)
	public:		
		struct ExportInfo : public EExp::ExportInfoBase
		{				
			awArrayStatic<awResourceID>	maTargetColorID;
			awResourceID				mTargetDepthID;
			awVec2U16						mvOrigin;
			awVec2U16						mvDim;
		};
		struct SerialUseOnly
		{							
			awArrayStatic<awResourceID>	maTargetColorID;
			awResourceID				mTargetDepthID;
		};
		virtual bool						ExportStart();
		static awResourceID			CallbackGetItemID(awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource, const EExp::ExportInfoBase* _pExportInfo, bool& _bExistOut);
		
		SerialUseOnly						mSerial;
		awVec2U16							mvOrigin;
		awVec2U16							mvDim;
	};

	awResourceID CreateGfxView( const awResourceID& _TargetColorID, const awResourceID& _TargetDepthID, const awVec2U16& _vDim=awVec2U16(9999,9999), const awVec2U16& _vOrigin=awVec2U16(0,0) );
	awResourceID CreateGfxView( const awArrayBase<awResourceID>& _aTargetColorID, const awResourceID& _TargetDepthID, const awVec2U16& _vDim=awVec2U16(9999,9999), const awVec2U16& _vOrigin=awVec2U16(0,0) );
}


#endif