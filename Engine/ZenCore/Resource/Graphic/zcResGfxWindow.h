#pragma once

namespace zxImGui	{ class zxRenderData; } //Forward declare
namespace zxNuklear { class zxRenderData; } //Forward declare
namespace zen { namespace zenWnd { class Window; } }
//namespace zcPerf { class}

#include zenHeaderRenderer(zcResGfxWindow)

namespace zcRes
{
	class GfxWindow : protected GfxWindow_HAL, public zenSig::zSlot
	{
	zenClassDeclare(GfxWindow, GfxWindow_HAL)
	public:
		enum eConstant{ keEventHistoryCount = 128 };		
		enum eEventType{ keEvtTyp_CPU, keEvtTyp_GPU, keEvtTyp__Count, keEvtTyp__Invalid=keEvtTyp__Count};

												GfxWindow();
		void									Resize(const zVec2U16& _vSize){ mvPendingResize = _vSize; } //! @todo safety make this multithread safe
		
		const GfxTarget2DRef&					GetBackbuffer();			
		void									FrameBegin();
		void									FrameEnd();
		zenSig::zSignalEmitter0&				GetSignalUIRender();
		zenInline const zcPerf::EventBaseRef&	GetHistoryEvent(eEventType _eEventType, zU32 _uIndex)const;
		zenInline zUInt							GetFrameCount()const;		
		zenWnd::Window*							mpMainWindowOS = nullptr; //! @todo urgent : temp hack until merged gfx + OS window
			
	protected:
		void									UIRenderCB();
		void									UIRenderFps();
		void									UIRenderFpsDetail();
		void									UIRenderEvents();
		void									UIRenderStatsHistogram(eEventType _eEventType, const char* _zHistoID, const char* _zTitle, const zVec3F& _vColor, float _fWidthRatio);	
		void									UIRenderEventTree(const zcPerf::EventBaseRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth=0 );
		bool									UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth, bool _bHeaderStartOpen);
		
		zUInt									muFrameCount			= 0;
		GfxTarget2DRef							mrBackbufferCurrent		= nullptr;		//!< BackBuffer where we should render final results for this frame
		zEngineRef<zxImGui::zxRenderData>		mrImGuiData				= nullptr;
		zEngineRef<zxNuklear::zxRenderData>		mrNuklearData			= nullptr;
		bool									mbUIShowFps				= true;
		bool									mbUIShowDetailFps		= false;
		bool									mbUIAutoDisplaySpike	= false;

		bool									mbUIEventShow[keEvtTyp__Count];			//!< True if window for a stats should be open
		bool									mbUIEventShowCurrent[keEvtTyp__Count];	//!< If we should display a window with latest stats each frame
		zcPerf::EventBaseRef					mrEventProfiling[keEvtTyp__Count];		//!< Which Event hierarchy to display for profiling
		zArrayStatic<zcPerf::EventBaseRef>		maEventHistory[keEvtTyp__Count];		//!< Hierarchical history of last X frame of events (with timing)

		zUInt									muEventValidIndex = 0;					//!< First valid root event index
		zU32									muEventValidCount = 0;					//!< Number of valid root events
		zcPerf::EventBaseRef					mrInvalidEvent;		

	public:
		using									Super::PerformResize;
		using									Super::RuntimeCreate;
		friend 									GfxWindow_HAL;
	};

	const zcPerf::EventBaseRef& GfxWindow::GetHistoryEvent(GfxWindow::eEventType _eEventType, zU32 _uIndex)const
	{
		zenAssert(_eEventType < keEvtTyp__Count);
		return  (_uIndex < muEventValidCount) ? maEventHistory[_eEventType][(muEventValidIndex+_uIndex)%keEventHistoryCount] : mrInvalidEvent;
	}

	zUInt GfxWindow::GetFrameCount()const
	{
		return muFrameCount;
	}
}
