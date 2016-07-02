#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zxImGui	{ class zxRenderData; } //Forward declare
namespace zxNuklear { class zxRenderData; } //Forward declare
namespace zen { namespace zenWnd { class Window; } }

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowResData, GfxWindowProxy, zcExp::ExporterNone>, public zenSig::zSlot
	{
	ZENClassDeclare(GfxWindow, TResource)
	enum eConstant{ keEventHistoryCount = 128 };		
	public:		
		enum eEventType{ keEvtTyp_CPU, keEvtTyp_GPU, keEvtTyp__Count, keEvtTyp__Invalid=keEvtTyp__Count};

												GfxWindow();
		void									Resize(const zVec2U16& _vSize){ mrProxy->mvPendingResize = _vSize; } //! @todo safety make this multithread safe
		void									SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor);
		const GfxRenderTargetRef&				GetBackbuffer();		
		void									FrameBegin();
		void									FrameEnd();
		zenSig::zSignalEmitter0&				GetSignalUIRender();
		ZENInline const zenPerf::zEventRef&		GetHistoryEvent(eEventType _eEventType, zU32 _uIndex)const;
		ZENInline zUInt							GetFrameCount()const;		
		zenWnd::Window*							mpMainWindowOS = nullptr; //! @todo urgent : temp hack until merged gfx + OS window
	
	protected:
		void									UIRenderCB();
		void									UIRenderFps();
		void									UIRenderFpsDetail();
		void									UIRenderEvents();
		void									UIRenderStatsHistogram(eEventType _eEventType, const char* _zHistoID, const char* _zTitle, const zVec3F& _vColor, float _fWidthRatio);	
		void									UIRenderEventTree(const zenPerf::zEventRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth=0 );
		bool									UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth, bool _bHeaderStartOpen);
		GfxRenderTargetRef						mrBackbufferColor[2];
		zUInt									muFrameCount			= 0;
		zEngineRef<zxImGui::zxRenderData>		mrImGuiData				= nullptr;
		zEngineRef<zxNuklear::zxRenderData>		mrNuklearData			= nullptr;
		bool									mbUIShowFps				= true;
		bool									mbUIShowDetailFps		= false;
		bool									mbUIAutoDisplaySpike	= false;

		bool									mbUIEventShow[keEvtTyp__Count];			//!< True if window for a stats should be open
		bool									mbUIEventShowCurrent[keEvtTyp__Count];	//!< If we should display a window with latest stats each frame
		zenPerf::zEventRef						mrEventProfiling[keEvtTyp__Count];		//!< Which Event hierarchy to display for profiling
		zArrayStatic<zenPerf::zEventRef>		maEventHistory[keEvtTyp__Count];		//!< Hierarchical history of last X frame of events (with timing)

		zU32									muEventValidIndex = 0;					//!< First valid root event index
		zU32									muEventValidCount = 0;					//!< Number of valid root events
		zenPerf::zEventRef						mrInvalidEvent;		
	};

	const zenPerf::zEventRef& GfxWindow::GetHistoryEvent(GfxWindow::eEventType _eEventType, zU32 _uIndex)const
	{
		ZENAssert(_eEventType < keEvtTyp__Count);
		if( _uIndex < muEventValidCount )
			return maEventHistory[_eEventType][(muEventValidIndex+_uIndex)%keEventHistoryCount];
		
		return mrInvalidEvent;
	}

	zUInt GfxWindow::GetFrameCount()const
	{
		return muFrameCount;
	}



}

#endif
