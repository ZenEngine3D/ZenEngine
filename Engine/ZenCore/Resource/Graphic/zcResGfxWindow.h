#pragma once
#ifndef __zCore_Res_Gfx_GfxWindow_h__
#define __zCore_Res_Gfx_GfxWindow_h__

namespace zxImGui {	class zxRenderData; } //Forward declare
namespace zen { namespace zenWnd { class Window; } }

namespace zcRes
{
	class GfxWindow : public TResource<GfxWindow, GfxWindowResData, GfxWindowProxy, zcExp::ExporterNone>, public zenSig::zSlot
	{
	ZENClassDeclare(GfxWindow, TResource)
	enum eConstant{ keEventHistoryCount = 128 };

	public:
												GfxWindow();
		void									Resize(const zVec2U16& _vSize){ mrProxy->mvPendingResize = _vSize; } //! @todo safety make this multithread safe
		void									SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor);
		const GfxRenderTargetRef&				GetBackbuffer();		
		void									FrameBegin();
		void									FrameEnd();
		zenSig::zSignalEmitter0&				GetSignalUIRender();
		ZENInline const zenPerf::zEventRef&		GetHistoryEventCPU(zU32 _uIndex)const;
		ZENInline const zenPerf::zEventRef&		GetHistoryEventGPU(zU32 _uIndex)const;
		ZENInline zUInt							GetFrameCount()const;		
		zenWnd::Window*							mpMainWindowOS = nullptr; //! @todo urgent : temp hack until merged gfx + OS window
	
	protected:
		void									UIRenderCB();
		void									UIRenderFps();
		void									UIRenderFpsDetail();
		void									UIRenderEvents();
		void									UIRenderStatsHistogram(const char* _zHistoID, const char* _zTitle, const zArrayStatic<zenPerf::zEventRef>& _aEvents, const zVec3F& _vColor, float _fWidthRatio);	
		void									UIRenderEventTree(const zenPerf::zEventRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth=0, bool _bGroupedSibbling=false );
		bool									UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth);
		GfxRenderTargetRef						mrBackbufferColor[2];
		zUInt									muFrameCount			= 0;
		zEngineRef<zxImGui::zxRenderData>		mrImGuiData				= nullptr;
		
		bool									mbUIShowFps				= true;
		bool									mbUIShowDetailFps		= false;
		bool									mbUIAutoDisplaySpike	= false;
		bool									mbUIEventProfilingShow	= false;
		bool									mbUIEventCurrentCPUShow	= false;
		bool									mbUIEventCurrentGPUShow	= false;
		
		zU32									muEventValidIndex		= 0;		//!< First valid root event index
		zU32									muEventValidCount		= 0;		//!< Number of valid root events
		zArrayStatic<zenPerf::zEventRef>		maEventHistoryCPU;		//!< Hierarchical history of last X frame of CPU events (with timing)
		zArrayStatic<zenPerf::zEventRef>		maEventHistoryGPU;		//!< Hierarchical history of last X frame of GPU events (with timing)
		zenPerf::zEventRef						mrEventProfiling;		//!< Event hierarchy to display for profiling
		zenPerf::zEventRef						mrInvalidEvent;		
	};

	const zenPerf::zEventRef& GfxWindow::GetHistoryEventCPU(zU32 _uIndex)const
	{
		if( _uIndex < muEventValidCount )
			return maEventHistoryCPU[(muEventValidIndex+_uIndex)%keEventHistoryCount];
		
		return mrInvalidEvent;
	}

	const zenPerf::zEventRef& GfxWindow::GetHistoryEventGPU(zU32 _uIndex)const
	{
		if( _uIndex < muEventValidCount )
			return maEventHistoryGPU[(muEventValidIndex+_uIndex)%keEventHistoryCount];
		
		return mrInvalidEvent;
	}

	zUInt GfxWindow::GetFrameCount()const
	{
		return muFrameCount;
	}



}

#endif
