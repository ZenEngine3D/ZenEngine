#include "zcCore.h"
#include "ZenExternal/UI/zxUIImgui.h"
#include "ZenEngine/ToDel/zeWndViewport.h"

namespace zcRes
{

struct EventHeaderInfo { float mfAlignX; const char* mzName; };
enum eCpuEventHeader{ keCpuEventHdr_Name, keCpuEventHdr_Time, keCpuEventHdr_Parent, keCpuEventHdr_Frame, keCpuEventHdr_GroupCount, keCpuEventHdr__Count};
static const EventHeaderInfo gaEventCpuHeader[keCpuEventHdr__Count]={ {20.f, "Event Name"}, {300.f, "Time"}, {375.f, "Parent"}, {425.f, "Frame"}, {475.f, "Count"} };

GfxWindow::GfxWindow()
{
	for(zUInt idx(0); idx<keEvtTyp__Count; ++idx)
	{
		maEventHistory[idx].resize(keEventHistoryCount);
		mbUIEventShowCurrent[idx] = false;
	}

	mrImGuiData	= zenMem::New<zxImGui::zxRenderData>();
	ConnectSignal_UIRender(mrImGuiData->msigRenderUI);
}

const GfxTarget2DRef& GfxWindow::GetBackbuffer() 
{		
	return mrBackbufferCurrent;
}

zenSig::zSignal<>& GfxWindow::GetSignalUIRender()
{
	zenAssert(mrImGuiData.IsValid());
	return mrImGuiData->msigRenderUI;
}

void GfxWindow::FrameBegin()
{
	Super::FrameBegin();

	++muFrameCount;
	char zFrameNameTemp[256];
	snprintf(zFrameNameTemp, sizeof(zFrameNameTemp), "Frame %i", static_cast<int>(muFrameCount));
	zStringHash32 zFrameName(zFrameNameTemp);
	zcMgr::GfxRender.FrameBegin(this);
	
	const zUInt uHistoryIndex = muFrameCount%keEventHistoryCount;
	maEventHistory[keEvtTyp_CPU][uHistoryIndex] = zcPerf::EventCPU::Create(zFrameName);
	maEventHistory[keEvtTyp_CPU][uHistoryIndex]->CPUStart();

	zenGfx::zCommandList rFrameContext = zenGfx::zCommandList::Create("Start Frame");
	maEventHistory[keEvtTyp_GPU][uHistoryIndex] = zcPerf::EventGPU::Create(zFrameName);
	maEventHistory[keEvtTyp_GPU][uHistoryIndex]->GPUStart(rFrameContext);
	rFrameContext.Submit();
}

void GfxWindow::FrameEnd()
{
	zenAssertMsg( this == zcGfx::grWindowRender.Get(), "Ending frame with different window than started");	
	
	//! @todo 2 cleanup this messy access
	// Editor doesn't have OS windows associated...
	if( mpMainWindowOS )
	{
		WindowInputState InputData;
		mpMainWindowOS->GetInput(InputData, 8); 
		mrImGuiData->mrRendertarget = GetBackbuffer();
		zxImGui::zxImGUIHelper::Get().Render(mrImGuiData, &InputData);
	}

	const zUInt uHistoryIndex = muFrameCount%keEventHistoryCount;
	maEventHistory[keEvtTyp_CPU][uHistoryIndex]->CPUStop();

	zenGfx::zCommandList rFrameContext = zenGfx::zCommandList::Create("End Frame");
	maEventHistory[keEvtTyp_GPU][uHistoryIndex]->GPUStop(rFrameContext);
	rFrameContext.Submit();

	zcMgr::GfxRender.FrameEnd();
	Super::FrameEnd();

	// Find first valid root event from history (values returned by GPU)
	muEventValidIndex	= (muFrameCount+2) % keEventHistoryCount; // Starts with oldest stats
	muEventValidCount	= 0;
	bool bFoundValid	= true;
	while( muEventValidCount < keEventHistoryCount && bFoundValid == true )
	{	
		zUInt EventIdx							= (muEventValidIndex+muEventValidCount)%keEventHistoryCount;
		const zcPerf::EventBaseRef& rEventGPU	= maEventHistory[keEvtTyp_GPU][EventIdx];
		bFoundValid								= rEventGPU.IsValid() && rEventGPU->GetElapsedMs() > 0.f; 
		muEventValidCount						+= bFoundValid ? 1 : 0;
	}
	muEventValidIndex = muEventValidIndex;
	
	// Auto display event profiling when there's a spike
	if( mbUIAutoDisplaySpike )
	{
		for(zU32 idxEventType(0); idxEventType < keEvtTyp__Count; ++idxEventType)
		{			
			if( !mrEventProfiling[idxEventType].IsValid() && muEventValidCount > 8 )
			{
				double fTimeTotal(0);
				for (zU32 idx(1); idx < muEventValidCount; ++idx)
					fTimeTotal += GetHistoryEvent((eEventType)idxEventType, idx)->GetElapsedMs();
			
				zcPerf::EventBaseRef rEvent	= GetHistoryEvent((eEventType)idxEventType, 0);
				double fAvgTime				= fTimeTotal/(muEventValidCount-1);
				double fSpikeTime			= fAvgTime * zenMath::Lerp(3.0, 1.25, static_cast<float>(fAvgTime/5.0) ); //Increase amount of time needed for spike, when running fast and small time change can easily be detected as one
				if( rEvent->GetElapsedMs() > fSpikeTime )
				{
					mbUIEventShow[idxEventType]		= true;
					mrEventProfiling[idxEventType]	= rEvent;
				}
			}
		}
	}
}

void GfxWindow::ConnectSignal_UIRender(zenSig::zSignal<>& _Signal)
{
	mSlotUIRender.Connect(_Signal, [&]()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if( ImGui::BeginMenu("ZenEngine") )
			{					
				ImGui::MenuItem("Performances",				nullptr, &mbUIShowFps);
				ImGui::MenuItem("Performances (details)",	nullptr, &mbUIShowDetailFps);
				ImGui::MenuItem("Profiling CPU",			nullptr, &mbUIEventShowCurrent[keEvtTyp_CPU]);
				ImGui::MenuItem("Profiling GPU",			nullptr, &mbUIEventShowCurrent[keEvtTyp_GPU]);
				ImGui::MenuItem("Profiling display spike",	nullptr, &mbUIAutoDisplaySpike);			
				ImGui::EndMenu();			
			}
			ImGui::EndMainMenuBar();
		}
		
		if( mbUIShowFps )
			UIRenderFps();

		if( mbUIShowDetailFps )
			UIRenderFpsDetail();

		UIRenderEvents();
	});
}

void GfxWindow::UIRenderFps( )
{
	const zcPerf::EventBaseRef& rEventCPU = GetHistoryEvent(keEvtTyp_CPU, 0);
	const zcPerf::EventBaseRef& rEventGPU = GetHistoryEvent(keEvtTyp_GPU, 0);
	if( rEventCPU.IsValid() && rEventGPU.IsValid() )
	{
		if (ImGui::BeginMainMenuBar())
		{
			float StartPos				= ImGui::GetCursorPos().x;
			float fTimeMsCPU			= static_cast<float>(rEventCPU->GetElapsedMs());
			float fTimeMsGPU			= static_cast<float>(rEventGPU->GetElapsedMs());
			ImGui::SameLine(ImGui::GetContentRegionAvailWidth()-100);	// Put fps text on the right side
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 1.0f, 1.0f));
			ImGui::Text("CPU:%6.02fms", fTimeMsCPU);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 1.0f, 0.8f, 1.0f));
			ImGui::Text("GPU:%6.02fms", fTimeMsGPU);
			ImGui::SameLine();
			ImGui::PopStyleColor(2);
			float EndPos				= ImGui::GetCursorPos().x;

			// Hack to add next menu item back beside previous one (since we moved fps infos at the far right)
			ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
			float PreviousSpacing		= CurrentStyle.ItemSpacing.x;
			CurrentStyle.ItemSpacing.x	= (StartPos-EndPos)+PreviousSpacing;
			ImGui::SameLine(0, -1);
			CurrentStyle.ItemSpacing.x	= PreviousSpacing;	
		}
		ImGui::EndMainMenuBar();
	}
}

void GfxWindow::UIRenderFpsDetail( )
{		
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetWindowContentRegionMin().y));
	ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
	ImGuiStyle PreviousStyle	= CurrentStyle;
	CurrentStyle.WindowPadding	= ImVec2(0, 0);
	CurrentStyle.FrameRounding	= 0.f;
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));	
	if ( ImGui::Begin("Detail Fps", nullptr, ImVec2(ImGui::GetIO().DisplaySize.x, 50), 0.1f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
	{	
		zenPerf::zScopedEventCpu EmiEvent("Stats Fps");							
		UIRenderStatsHistogram(keEvtTyp_CPU, "##FramerateCPU", "CPU: %6.02fms (%6.02ffps)", zVec3F(0.2f, 0.2f, 0.8f), 0.48f );
		UIRenderStatsHistogram(keEvtTyp_GPU, "##FramerateGPU", "GPU: %6.02fms (%6.02ffps)", zVec3F(0.2f, 0.8f, 0.2f), 0.48f );
		ImGui::End();
	}	
	ImGui::PopStyleColor();	
	ImGui::GetStyle() = PreviousStyle;
}

void GfxWindow::UIRenderStatsHistogram(eEventType _eEventType, const char* _zHistoID, const char* _zTitle, const zVec3F& _vColor, float _fWidthRatio)
{
	zenAssert( _eEventType < keEvtTyp__Count );
	zArrayDyn<float>	aFrameMs;
	zcRes::GfxWindowRef rWindow						= this;
	const zUInt uFrameIndex							= rWindow->GetFramesize();
	int uStatCount									= 0;
	float fMinFrameMs								= 9999.f;
	const zArrayDyn<zcPerf::EventBaseRef>& aEvents	= maEventHistory[_eEventType];
	aFrameMs.resize(muEventValidCount);
	for (zUInt idx(0); idx < muEventValidCount; ++idx)
	{
		const zUInt uHistoryIndex	= (muEventValidIndex + idx) % keEventHistoryCount;
		float frameMs				= aEvents[uHistoryIndex].IsValid() ? static_cast<float>(aEvents[uHistoryIndex]->GetElapsedMs()) : 0.f;
		if (frameMs > 0)
		{
			aFrameMs[uStatCount++]	= frameMs;
			fMinFrameMs				= zenMath::Min(fMinFrameMs, frameMs);
		}
	}

	if (uStatCount)
	{
		char zFpsTitle[64];
		snprintf(zFpsTitle, sizeof(zFpsTitle), _zTitle, fMinFrameMs, 1000.f / fMinFrameMs);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.00f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(_vColor.r, _vColor.g, _vColor.b, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(_vColor.r, _vColor.g, _vColor.b, 0.9f));
		ImGui::SameLine();
		ImGui::PlotHistogram(_zHistoID, aFrameMs.Data(), uStatCount, 0, zFpsTitle, 0.0f, (fMinFrameMs < 16.f ? fMinFrameMs*2.f : 100.f), ImVec2(ImGui::GetWindowContentRegionWidth() * _fWidthRatio, 50));
		ImGui::PopStyleColor(3);

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left))
		{
			ImVec2 vImMouse = ImGui::GetMousePos();
			const ImGuiStyle& style = ImGui::GetStyle();
			float minX = ImGui::GetItemRectMin().x + style.FramePadding.x;
			float maxX = ImGui::GetItemRectMax().x - style.FramePadding.x;
			float ratio = (vImMouse.x - minX) / (maxX - minX);
			if (ratio > 0.f && ratio < 1.f)
			{
				zUInt uEventIndex				= int(ratio*uStatCount);
				zUInt uHistoryIndex				= (uEventIndex + muEventValidIndex) % uStatCount;
				mbUIEventShow[_eEventType]		= true;
				mrEventProfiling[_eEventType]	= aEvents[uHistoryIndex];
			}
		}
	}
}

void GfxWindow::UIRenderEvents( )
{
	const char* azWindowTile[]={"CPU Profiling", "GPU Profiling"};
	zenStaticAssert(zenArrayCount(azWindowTile) == keEvtTyp__Count);

	ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
	ImGuiStyle PreviousStyle	= CurrentStyle;
	CurrentStyle.WindowPadding	= ImVec2(0, 0);
	CurrentStyle.FrameRounding	= 0.f;	
	for(zUInt idxEvent(0); idxEvent<keEvtTyp__Count; ++idxEvent)
	{
		if( mbUIEventShow[idxEvent] )
		{
			zcPerf::EventBaseRef rEvent	= mrEventProfiling[idxEvent];
			rEvent						= !rEvent.IsValid() && mbUIEventShowCurrent[idxEvent] ? GetHistoryEvent((eEventType)idxEvent, 0) : rEvent;
			if (rEvent.IsValid())
			{
				if (ImGui::Begin(azWindowTile[idxEvent], &mbUIEventShow[idxEvent], ImVec2(ImGui::GetIO().DisplaySize.x / 2.f, 0), 0.8f, ImGuiWindowFlags_NoCollapse))
				{
					zUInt uItemCount(0);
					ImGui::Text("");
					for (zUInt idx(0); idx < keCpuEventHdr__Count; ++idx)
					{
						ImGui::SameLine(gaEventCpuHeader[idx].mfAlignX);
						ImGui::Text(gaEventCpuHeader[idx].mzName);
					}
					UIRenderEventTree(rEvent, rEvent->GetElapsedMs(), rEvent->GetElapsedMs(), uItemCount);
					ImGui::End();
				}
			}
		}
		else
		{
			mbUIEventShowCurrent[idxEvent]	= false;
			mrEventProfiling[idxEvent]		= nullptr;
		}
	}
	ImGui::GetStyle() = PreviousStyle;
}



void GfxWindow::UIRenderEventTree(const zcPerf::EventBaseRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth )
{
	zUInt uSibblingCount		= 0;
	zUInt uChildCount			= 0;
	double fTimeChilds			= 0;
	double fSibblingTime		= 0;

	// Event with children
	if( _rProfilEvent->GetFirstChild().IsValid() )
	{
		zcPerf::EventBaseRef rEvent = _rProfilEvent->GetFirstChild();
		while (rEvent.IsValid())
		{
			fTimeChilds += rEvent->GetElapsedMs();
			rEvent		= rEvent->GetNext();
			++uChildCount;
		}
	}	
	// Event without children node
	else
	{
		// See if we're first event of many with same name and parent (to group them)		
		uSibblingCount				= 1;
		fSibblingTime				= _rProfilEvent->GetElapsedMs();
		zcPerf::EventBaseRef rEvent	= _rProfilEvent->GetNext();
		while (rEvent.IsValid())
		{
			if (rEvent->GetName() == _rProfilEvent->GetName() && !rEvent->GetFirstChild().IsValid())
			{
				fSibblingTime += rEvent->GetElapsedMs();
				++uSibblingCount;
			}
			rEvent = rEvent->GetNext();
		}

		// Discard event sharing name, since displayed by 1st item
		rEvent = _rProfilEvent->GetPrev();
		while (rEvent.IsValid())
		{
			if (rEvent->GetName() == _rProfilEvent->GetName() && !rEvent->GetFirstChild().IsValid())
				return;
			rEvent = rEvent->GetPrev();
		}
	}

	bool bOpenNode = UIRenderEventTreeItem(_rProfilEvent->GetName().mzName, uSibblingCount > 1 ? fSibblingTime : _rProfilEvent->GetElapsedMs(), _fTotalTime, _fParentTime, _uItemCount, uChildCount + uSibblingCount, _uDepth, uChildCount>0);
	++_uItemCount;

	float fTimeUnscoped = static_cast<float>(_rProfilEvent->GetElapsedMs()-fTimeChilds);
	if( bOpenNode && fTimeUnscoped > 0.001f && uSibblingCount <= 1 )
	{
		UIRenderEventTreeItem("Untagged", fTimeUnscoped, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, 0, _uDepth+1, false);
		++_uItemCount;
	}

	if( bOpenNode )
	{
		if( uSibblingCount > 1 )
		{
			zcPerf::EventBaseRef rEventSibbling = _rProfilEvent;
			while( rEventSibbling.IsValid() )
			{		
				if( rEventSibbling->GetName() == _rProfilEvent->GetName() )
					UIRenderEventTreeItem(rEventSibbling->GetName().mzName, _rProfilEvent->GetElapsedMs(), _fTotalTime, fSibblingTime, _uItemCount, 0, _uDepth+1, false);

				rEventSibbling = rEventSibbling->GetNext();
			}
		}
		else
		{
			zcPerf::EventBaseRef rEventChild = _rProfilEvent->GetFirstChild();			
			while( rEventChild.IsValid()  )
			{				
				UIRenderEventTree(rEventChild, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, _uDepth+1  );
				rEventChild = rEventChild->GetNext();
			}
		}
	}
}

//=================================================================================================
// UIRenderEventTreeItem
//-------------------------------------------------------------------------------------------------
//! @param	_zEventName		- Event name, used for tree node text
//! @param	_fEventTime		- Time length of event (in seconds)
//! @param	_fTotalTime		- Frame time
//! @param	_fParentTime	- Parent event time length
//! @param	_uItemCount		- Number of displayed lines so far
//! @param	_uChildCount	- Number of child nodes
//! @param	_uDepth			- Tree node depth
//! @return					- true if tree node is open
//=================================================================================================
bool GfxWindow::UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth, bool _bHeaderStartOpen)
{
	const bool bIsHeader	= _uChildCount > 1 && _fEventTime > 0.01f;
	bool bOpenNode			= false;
	float fOpacityMul		= bIsHeader ? 2.5f : 1.f;

	ImGui::PushStyleColor(ImGuiCol_Header, _uItemCount % 2 ?		ImVec4(0.4f, 0.2f, 0.2f, 0.15f*fOpacityMul) : ImVec4(0.2f, 0.2f, 0.4f, 0.15f*fOpacityMul));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, _uItemCount % 2 ? ImVec4(0.4f, 0.2f, 0.2f, 0.8f*fOpacityMul)	: ImVec4(0.2f, 0.2f, 0.4f, 0.8f*fOpacityMul));
	if( bIsHeader )
	{		
		bOpenNode = ImGui::CollapsingHeader("", _zEventName, true, _bHeaderStartOpen);
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_Name].mfAlignX + _uDepth*20);
	}
	else
	{
		ImGui::Selectable("", true, ImGuiSelectableFlags_SpanAllColumns);
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_Name].mfAlignX + _uDepth*20 - 20);
		ImGui::Bullet();
	}

	ImGui::Text(_zEventName);
	if( _fEventTime > 0.0f )
	{
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_Time].mfAlignX);
		ImGui::Text("%06.3fms", _fEventTime);
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_Parent].mfAlignX);
		ImGui::Text("%5.3f", _fEventTime / _fParentTime);
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_Frame].mfAlignX);
		ImGui::Text("%5.3f", _fEventTime / _fTotalTime);
	}

	if( _uChildCount > 0 )
	{
		ImGui::SameLine(gaEventCpuHeader[keCpuEventHdr_GroupCount].mfAlignX);
		ImGui::Text("%03i", _uChildCount);
	}

	ImGui::PopStyleColor(2);
	++_uItemCount;
	return bOpenNode;
}

}