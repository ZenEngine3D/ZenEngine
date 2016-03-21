#include "zeEngine.h"
#include <Engine/ThirdParty/imgui/imgui.h> //! @todo Urgent remove this
namespace zen { namespace zenSys {	

static zEngineInstance*	gpActiveEngine(nullptr);

void LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const* argv)
{	
	if( _pEngineInstance->Init() )
	{
		ZENAssert(!gpActiveEngine);
		gpActiveEngine = _pEngineInstance;
		while( !gpActiveEngine->IsDone() && zbSys::IsSystemActive() )
		{
			gpActiveEngine->MainLoop();
		}
	}	
	gpActiveEngine->Destroy();
}

zEngineInstance* GetEngineInstance()
{
	ZENAssert(gpActiveEngine);
	return gpActiveEngine;
}

bool zEngineInstance::Init()
{
	marHistoryEvents.SetCount( 128 );
	for(zUInt idx(0), count(marHistoryEvents.Count()); idx<count; ++idx)
	{
		zenPerf::zEventRef rRootFrameEvent = zenPerf::zEventRef::CreateCPU("RootInit");
		rRootFrameEvent.Start();
		rRootFrameEvent.Stop();
		marHistoryEvents[idx] = rRootFrameEvent;
	}
	return zbSys::EngineStart();
}

void zEngineInstance::Destroy()
{
	mrMainWindowGfx = nullptr;
	zenDelnullptr(mpMainWindowOS);		
	zbSys::EngineStop();
	gpActiveEngine = nullptr;
}

void zEngineInstance::MainLoop()
{
	zenPerf::zEventRef rRootFrameEvent = zenPerf::zEventRef::CreateCPU("Root");	
	marHistoryEvents[muFrameCount % marHistoryEvents.Count()] = rRootFrameEvent; 
	rRootFrameEvent->Start();
	
//	zcMgr::Job.Update();
	if( mpMainWindowOS )
	{
		zVec2U16 vWindowSize = mpMainWindowOS->GetSize();
		if( muWindowSize != vWindowSize )
		{
			muWindowSize = vWindowSize;
			mrMainWindowGfx.Resize(muWindowSize);
		}
	}
	
	{
		zenPerf::zScopedEventCpu EmitEvent("Update");
		{
			zenPerf::zScopedEventCpu EmitEvent("Update Start");
			zcMgr::Updater.Update(zenConst::keUpdt_FrameStart);
			msigUpdate.Emit(zenConst::keUpdt_FrameStart);
		}
		{
			zenPerf::zScopedEventCpu EmitEvent("Engine::Update");
			Update(); //! @todo Clean remove this and use signals
		}
		{
			zenPerf::zScopedEventCpu EmitEvent("Update End");
			zcMgr::Updater.Update(zenConst::keUpdt_FrameEnd);
			msigUpdate.Emit(zenConst::keUpdt_FrameEnd);
		}
		{
			zenPerf::zScopedEventCpu EmitEvent("Resources Release");
			zenRes::zResource::ReleaseUnused();
		}
	}

	rRootFrameEvent->Stop();
	++muFrameCount;
}

void zEngineInstance::Update()
{	
}

void zEngineInstance::CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos)
{
	ZENAssert(gpActiveEngine==nullptr);
	ZENAssert(mpMainWindowOS == nullptr);
	mpMainWindowOS				= zenNewDefault zenWnd::Window(L"MainWindow", _vDim);
	mpMainWindowOS->Initialize();
	mrMainWindowGfx				= zenRes::zGfxWindow::Create( mpMainWindowOS->GetHandle() );
	zcRes::GfxWindowRef rWindow = mrMainWindowGfx;
	rWindow->mpMainWindowOS		= mpMainWindowOS;
	muWindowSize				= _vDim;
	rWindow->GetSignalUIRender().Connect(*this, &zEngineInstance::UIRenderCB);
}

bool zSampleEngineInstance::IsDone()
{
	return mbDone;
}

void zSampleEngineInstance::SetDone()
{
	mbDone = true;
}

void zSampleEngineInstance::UIRender()
{	
	Super::UIRender();

	static bool	sbShowImGUITest = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Sample"))
		{
			ImGui::MenuItem("ImGUI Demo", nullptr, &sbShowImGUITest);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (sbShowImGUITest)
		ImGui::ShowTestWindow();
}

void zEngineInstance::UIRenderCB()
{
	UIRender();	
}

void zEngineInstance::UIRender()
{
	static bool sbShowFps		= true;
	
	if (ImGui::BeginMainMenuBar())
	{
		if( ImGui::BeginMenu("Zen Engine") )
		{		
			if( ImGui::BeginMenu("Debug") )
			{
				ImGui::MenuItem("Fps", nullptr, &sbShowFps);				
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	
	if( sbShowFps )
		UIRenderStats();	
}


void zEngineInstance::UIRenderStats( )
{		
	static zI16 suStatsGPUFrame = -1;
	ImVec2 vPos = ImVec2(0, ImGui::GetWindowContentRegionMin().y);
	ImGui::SetNextWindowPos(vPos);

	ImGuiStyle& CurrentStyle = ImGui::GetStyle();
	ImGuiStyle PreviousStyle = CurrentStyle;
	CurrentStyle.WindowPadding = ImVec2(0, 0);
	CurrentStyle.FrameRounding = 0.f;
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	
	if ( ImGui::Begin("Fps", nullptr, ImVec2(ImGui::GetIO().DisplaySize.x, 0), 0.1f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{	
		zenPerf::zScopedEventCpu EmiEvent("Stats Fps");
		char zFpsGpuTitle[64];
		const zArrayStatic<float>&	aFrameMs = zcMgr::GfxRender.GetFrameTimeHistory();
		float fFrameAvg = zcMgr::GfxRender.GetFrameTimeAvg();
		int valOffet = (zcMgr::GfxRender.GetStatsFrame() + 1) % aFrameMs.Count();
		sprintf(zFpsGpuTitle, "GPU: %.1fms (%.1ffps)", fFrameAvg, 1000.f / fFrameAvg);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.00f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.8f, 0.2f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(0.2f, 0.8f, 0.2f, 0.5f));
		ImGui::PlotHistogram("##FramerateGPU", aFrameMs.First(), aFrameMs.Count(), valOffet, zFpsGpuTitle, 0.0f, fFrameAvg*2.f, ImVec2(ImGui::GetContentRegionAvailWidth() / 2.f, 50));
		ImGui::PopStyleColor(2);

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left))
		{
			zcMgr::GfxRender.SetStatsUpdate(!zcMgr::GfxRender.GetStatsUpdate());
		}
		
		zArrayStatic<float> afCpuFrame;
		const zUInt uEventCount	= marHistoryEvents.Count();
		fFrameAvg		= 0;
		afCpuFrame.SetCount(uEventCount);
		
		for(zUInt idx(0); idx<uEventCount -1; ++idx)
		{
			afCpuFrame[idx] = marHistoryEvents[(idx+muFrameCount+2)% uEventCount]->GetElapsedMs();
			fFrameAvg += afCpuFrame[idx];
		}
		fFrameAvg /= uEventCount - 1;

		char zFpsCpuTitle[64];
		sprintf(zFpsCpuTitle, "CPU: %.1fms (%.1ffps)", fFrameAvg, 1000.f / fFrameAvg);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.2f, 0.8f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(0.2f, 0.2f, 0.8f, 0.5f));
		ImGui::PlotHistogram("##FramerateCPU", afCpuFrame.First(), afCpuFrame.Count(), 0, zFpsCpuTitle, 0.0f, fFrameAvg*2.f, ImVec2(ImGui::GetContentRegionAvailWidth(), 50));
		ImGui::PopStyleColor(3);

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left))
		{			
			ImVec2 vImMin = ImGui::GetItemRectMin();
			ImVec2 vImMax = ImGui::GetItemRectMax();
			ImVec2 vImMouse = ImGui::GetMousePos();
			const ImGuiStyle& style = ImGui::GetStyle();
			float minX = vImMin.x + style.FramePadding.x;
			float maxX = vImMax.x - style.FramePadding.x;
			float ratio = (vImMouse.x - minX) / (maxX - minX);
			if (ratio > 0.f && ratio < 1.f)
			{
				int sEventIndex		= int(ratio*uEventCount);
				mbUIFrameStatsShow	= true;
				mrUIFrameStatsEvent	= marHistoryEvents[sEventIndex];
			}
		}

		ImGui::End();
	}
	
	ImGui::PopStyleColor();


	if( mbUIFrameStatsShow == false )
		mrUIFrameStatsEvent = zenPerf::zEventRef();	

	if(mbUIFrameStatsShow && ImGui::Begin("Profiling", &mbUIFrameStatsShow, ImVec2(ImGui::GetIO().DisplaySize.x/2.f, 0), 0.8f, ImGuiWindowFlags_NoCollapse) )
	{
		zenPerf::zScopedEventCpu EmiEvent("Stats Profiling");
		zInt sDepth(0);
		zUInt uItemCount(0);
		//zenPerf::zEventRef rProfilEvent = marHistoryEvents[0];
		
		ImGui::Text(" ");
		ImGui::Columns(4, nullptr, false);
		ImGui::PushStyleColor(ImGuiCol_Header,			ImVec4(0.1f, 0.25f, 0.1f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered,	ImVec4(0.1f, 0.25f, 0.1f, 0.9f));
		//ImGui::PushStyleColor(ImGuiCol_WindowBg,		ImVec4(0.0f, 0.25f, 0.1f, 0.9f));

		ImGui::SetColumnOffset(1, 20);
		ImGui::SetColumnOffset(2, 20);
		ImGui::SetColumnOffset(3, 300);
		ImGui::Selectable("", true, ImGuiSelectableFlags_SpanAllColumns);
		ImGui::NextColumn();
		ImGui::NextColumn();		
		ImGui::Text("Event name");		
		ImGui::NextColumn();
		ImGui::Text("Time      Parent  Frame");
		ImGui::Columns(1);
		ImGui::PopStyleColor(2);
		UIRenderEventTree(mrUIFrameStatsEvent, mrUIFrameStatsEvent->GetElapsedMs(), mrUIFrameStatsEvent->GetElapsedMs(), uItemCount);
		ImGui::End();
	}

	CurrentStyle = PreviousStyle;
	//printf("Hovering %i\n", int(ratio*aFrameMs.Count()));	
}

void zEngineInstance::UIRenderEventTree(const zenPerf::zEventRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth, bool _bGroupedSibbling)
{
	zUInt uSibblingCount		= 0;
	zUInt uChildCount			= 0;
	double fTimeChilds			= 0;
	double fSibblingTime		= 0;
	// If finds another event with same ID before this one, cancel display
	if( !_bGroupedSibbling && !_rProfilEvent.GetFirstChild().IsValid() )
	{
		zenPerf::zEventRef rEvent = _rProfilEvent.GetPrev();
		while( rEvent.IsValid() )
		{			
			if(rEvent->mzEventName == _rProfilEvent->mzEventName && !rEvent.GetFirstChild().IsValid())
				return;		
			rEvent = rEvent.GetPrev();
		}
	
		// See if we're first event of many with same parent (to group them)
		fSibblingTime	= _rProfilEvent->GetElapsedMs();
		rEvent			= _rProfilEvent.GetNext();
		while( rEvent.IsValid() )
		{		
			if(rEvent->mzEventName == _rProfilEvent->mzEventName && !rEvent.GetFirstChild().IsValid() )
			{
				fSibblingTime += rEvent->GetElapsedMs();
				++uSibblingCount;
			}
			rEvent = rEvent.GetNext();
		}
	}
	else
	{
		zenPerf::zEventRef rEvent = _rProfilEvent.GetFirstChild();
		while( rEvent.IsValid() )
		{
			fTimeChilds += rEvent->GetElapsedMs();
			rEvent		= rEvent.GetNext();
			++uChildCount;
		}
	}	

	bool bOpenNode = UIRenderEventTreeItem(_rProfilEvent->mzEventName.mzName, uSibblingCount > 0 ? fSibblingTime : _rProfilEvent->GetElapsedMs(), _fTotalTime, _fParentTime, _uItemCount, uChildCount + uSibblingCount, _uDepth);
	++_uItemCount;

	if( bOpenNode && uChildCount > 0 )
	{
		UIRenderEventTreeItem("Unnamed", _rProfilEvent->GetElapsedMs()-fTimeChilds, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, 0, _uDepth+1);
		++_uItemCount;
	}

	if( bOpenNode )
	{
		if( uSibblingCount > 0 )
		{
			zenPerf::zEventRef rEventSibbling = _rProfilEvent.GetNext();
			while( rEventSibbling.IsValid() )
			{		
				//recalculate sibling time total
				if(rEventSibbling->mzEventName == _rProfilEvent->mzEventName && !rEventSibbling.GetFirstChild().IsValid() )
					UIRenderEventTree(rEventSibbling, _fTotalTime, _fParentTime, _uItemCount, _uDepth+1, true );
				rEventSibbling = rEventSibbling.GetNext();
			}
		}
		else
		{
			zenPerf::zEventRef rEventChild = _rProfilEvent.GetFirstChild();			
			while( rEventChild.IsValid()  )
			{				
				UIRenderEventTree(rEventChild, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, _uDepth+1 );
				rEventChild = rEventChild.GetNext();
			}
		}
	}


#if 0
	zenPerf::zEventRef rChildCur	= _rProfilEvent.GetFirstChild();
	bool bHasChild					= rChildCur.IsValid();
	bool bOpenNode					= false;
 
	float fLineWidth = ImGui::GetWindowWidth();
	ImGui::Columns(4, nullptr, false);	
	ImGui::SetColumnOffset(1, 20);
	ImGui::SetColumnOffset(2, 20+_uDepth * 20);
	ImGui::SetColumnOffset(3, 300 );

	if (bHasChild)
	{
		ImGui::PushID(_uItemCount);
		bOpenNode = ImGui::TreeNode("");
		ImGui::PopID();
		if(bOpenNode )
			ImGui::TreePop();		
	}
	ImGui::NextColumn();
	
	ImGui::PushStyleColor(ImGuiCol_Header, _uItemCount % 2 ?		ImVec4(0.0f, 0.0f, 0.0f, 0.0f) : ImVec4(0.2f, 0.2f, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, _uItemCount % 2 ? ImVec4(0.2f, 0.4f, 0.2f, 0.4f) : ImVec4(0.2f, 0.4f, 0.2f, 0.4f));
	ImGui::Selectable("", true, ImGuiSelectableFlags_SpanAllColumns);
	ImGui::PopStyleColor(2);
	
	ImGui::NextColumn();
	ImGui::Text(_rProfilEvent->mzEventName.mzName);
	ImGui::NextColumn();
	++_uItemCount;
	
	double fRatioParent = _rProfilEvent->GetElapsedMs() / _fParentTime;
	double fRatioTotal	= _rProfilEvent->GetElapsedMs() / _fTotalTime;
	ImGui::Text("%6.3fms  %5.3f   %5.3f", _rProfilEvent->GetElapsedMs(), fRatioParent, fRatioTotal);
	ImGui::Columns(1);

	if( bOpenNode )
	{
		double fChildTime(0);
		while(rChildCur.IsValid())
		{
			fChildTime += rChildCur->GetElapsedMs();
			rChildCur = rChildCur.GetNext();
		}
		
		rChildCur = _rProfilEvent.GetFirstChild();
		while (bOpenNode && rChildCur.IsValid()  )
		{
			if( true || rChildCur.GetFirstChild().IsValid() )
			{
				UIRenderEventTree(rChildCur, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, _uDepth+1 );				
			}
			else
			{
			/*
				double fRatioParent = _rProfilEvent->GetElapsedMs() / _fParentTime;
				double fRatioTotal	= _rProfilEvent->GetElapsedMs() / _fTotalTime;
				ImGui::Text("%5.2fms  %5.3f   %5.3f", _rProfilEvent->GetElapsedMs(), fRatioParent, fRatioTotal);
				ImGui::Columns(1);
				++_uItemCount;*/
			}

			fChildTime += rChildCur->GetElapsedMs();
			rChildCur = rChildCur.GetNext();
		}

		
	}		
	
	#endif
}

bool zEngineInstance::UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth)
{
	bool bOpenNode			= false;
	float fLineWidth		= ImGui::GetWindowWidth();
	ImGui::Columns(4, nullptr, false);	
	ImGui::SetColumnOffset(1, 20);
	ImGui::SetColumnOffset(2, 20+_uDepth * 20);
	ImGui::SetColumnOffset(3, 300 );

	if( _uChildCount > 0 )
	{
		ImGui::PushID(_uItemCount);
		bOpenNode = ImGui::TreeNode("");
		ImGui::PopID();
		if(bOpenNode )
			ImGui::TreePop();		
	}
	ImGui::NextColumn();
	
	//----------------------
	ImGui::PushStyleColor(ImGuiCol_Header, _uItemCount % 2 ?		ImVec4(0.0f, 0.0f, 0.0f, 0.0f) : ImVec4(0.2f, 0.2f, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, _uItemCount % 2 ? ImVec4(0.2f, 0.4f, 0.2f, 0.4f) : ImVec4(0.2f, 0.4f, 0.2f, 0.4f));
	ImGui::Selectable("", true, ImGuiSelectableFlags_SpanAllColumns);
	ImGui::PopStyleColor(2);
	
	ImGui::NextColumn();
	ImGui::Text(_zEventName);
	ImGui::NextColumn();
	++_uItemCount;
	
	double fRatioParent = _fEventTime / _fParentTime;
	double fRatioTotal	= _fEventTime / _fTotalTime;
	ImGui::Text("%6.3fms  %5.3f   %5.3f", _fEventTime, fRatioParent, fRatioTotal);
	ImGui::Columns(1);
	//----------------------

	return bOpenNode;
}

}} // namespace zen, zenRes
