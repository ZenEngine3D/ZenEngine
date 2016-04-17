#include "zcCore.h"
#include "Engine/ZenExternal/UI/zxUIImgui.h"
#include "Engine/ZenEngine/ToDel/zeWndViewport.h"
#include <Engine/ThirdParty/imgui/imgui.h> //! @todo Urgent remove this
namespace zcRes
{

GfxWindow::GfxWindow()
{
	mrImGuiData = new(static_cast<zenMem::zAllocator*>(nullptr), 16) zxImGui::zxRenderData; //! @todo Urgent auto find alignment needs...
	maEventHistoryCPU.SetCount(keEventHistoryCount);
	maEventHistoryGPU.SetCount(keEventHistoryCount);
	mrImGuiData->msigRenderUI.Connect(*this, &GfxWindow::UIRenderCB);
}

void GfxWindow::SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor) 
{ 
	ZENAssert(_uBackbufferIndex < ZENArrayCount(mrBackbufferColor));
	mrBackbufferColor[_uBackbufferIndex] = _rBackbufferColor;
	
}

const GfxRenderTargetRef& GfxWindow::GetBackbuffer() 
{		
	return mrBackbufferColor[ muFrameCount % ZENArrayCount(mrBackbufferColor) ];
}

zenSig::zSignalEmitter0& GfxWindow::GetSignalUIRender()
{
	ZENAssert(mrImGuiData.IsValid());
	return mrImGuiData->msigRenderUI;
}

void GfxWindow::FrameBegin()
{
	char zFrameNameTemp[256];
	sprintf(zFrameNameTemp, "Frame %06i", muFrameCount++);
	zStringHash32 zFrameName(zFrameNameTemp);
	zcMgr::GfxRender.FrameBegin(this);
	
	const zUInt uHistoryIndex = muFrameCount%keEventHistoryCount;
	maEventHistoryCPU[uHistoryIndex] = zcPerf::EventCPU::Create(zFrameName);
	maEventHistoryCPU[uHistoryIndex]->Start();
	maEventHistoryGPU[uHistoryIndex] = zcPerf::EventGPU::Create(zFrameName);
	maEventHistoryGPU[uHistoryIndex]->Start();
}

void GfxWindow::FrameEnd()
{
	ZENAssertMsg( this == zcGfx::gWindowRender.Get(), "Ending frame with different window than started");	
	WindowInputState InputData;
	mpMainWindowOS->GetInput(InputData, 8); //! @todo urgent cleanup this messy access
	mrImGuiData->mrRendertarget = GetBackbuffer();
	zxImGui::zxImGUIHelper::Get().Render(mrImGuiData, &InputData);
	const zUInt uHistoryIndex = muFrameCount%keEventHistoryCount;		
	maEventHistoryCPU[uHistoryIndex]->Stop();
	maEventHistoryGPU[uHistoryIndex]->Stop();
	zcMgr::GfxRender.FrameEnd();

	// Find first valid root event from history (values returned by GPU)
	muEventValidIndex	= muFrameCount + keEventHistoryCount;
	muEventValidCount	= keEventHistoryCount;
	bool bFoundValid	= false;
	while( muEventValidIndex >= 1 && !bFoundValid )
	{
		--muEventValidIndex;
		--muEventValidCount;
		const zenPerf::zEventRef& rEventGPU = maEventHistoryGPU[ muEventValidIndex%keEventHistoryCount ];
		bFoundValid							= rEventGPU.IsValid() && rEventGPU->GetElapsedMs() > 0.f; 
	}
	muEventValidIndex = muEventValidIndex % keEventHistoryCount;
}

void GfxWindow::UIRenderCB()
{
	if (ImGui::BeginMainMenuBar())
	{
		if( ImGui::BeginMenu("ZenEngine") )
		{					
			ImGui::MenuItem("Performances", nullptr, &mbUIShowFps);
			ImGui::MenuItem("Performances (details)", nullptr, &mbUIShowDetailFps);
			ImGui::MenuItem("Profiling GPU", nullptr, &mbUIEventCurrentGPUShow);
			ImGui::MenuItem("Profiling CPU", nullptr, &mbUIEventCurrentCPUShow);
			//ImGui::MenuItem("Profiling display spike", nullptr, &mbUIAutoDisplaySpike);			
			ImGui::EndMenu();			
		}
		ImGui::EndMainMenuBar();
	}
		
	if( mbUIShowFps )
		UIRenderFps();

	if( mbUIShowDetailFps )
		UIRenderFpsDetail();

	UIRenderEvents();
}

void GfxWindow::UIRenderFps( )
{
	const zenPerf::zEventRef& rEventCPU = GetHistoryEventCPU(0);
	const zenPerf::zEventRef& rEventGPU = GetHistoryEventGPU(0);
	if( rEventCPU.IsValid() && rEventGPU.IsValid() )
	{
		if (ImGui::BeginMainMenuBar())
		{
			float StartPos				= ImGui::GetCursorPos().x;
			float fTimeMsCPU			= rEventCPU->GetElapsedMs();
			float fTimeMsGPU			= rEventGPU->GetElapsedMs();			
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
		UIRenderStatsHistogram("##FramerateCPU", "CPU: %6.02fms (%6.02ffps)", maEventHistoryCPU, zVec3F(0.2f, 0.2f, 0.8f), 0.48f );
		UIRenderStatsHistogram("##FramerateGPU", "GPU: %6.02fms (%6.02ffps)", maEventHistoryGPU, zVec3F(0.2f, 0.8f, 0.2f), 0.48f );
		ImGui::End();
	}	
	ImGui::PopStyleColor();	
	ImGui::GetStyle() = PreviousStyle;
}

void GfxWindow::UIRenderEvents()
{
	zenPerf::zEventRef rEvent;
	bool* pbLinkedBool = nullptr;
	if( mbUIEventProfilingShow && mrEventProfiling.IsValid() )
	{
		rEvent			= mrEventProfiling;
		pbLinkedBool	= &mbUIEventProfilingShow;
	}
	else if( mbUIEventCurrentCPUShow )
	{
		rEvent			= GetHistoryEventCPU(0);
		pbLinkedBool	= &mbUIEventCurrentCPUShow;
	}
	else if( mbUIEventCurrentGPUShow )
	{
		rEvent			= GetHistoryEventGPU(0);
		pbLinkedBool	= &mbUIEventCurrentGPUShow;
	}

	if( rEvent.IsValid() )
	{
		ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
		ImGuiStyle PreviousStyle	= CurrentStyle;
		CurrentStyle.WindowPadding	= ImVec2(0, 0);
		CurrentStyle.FrameRounding	= 0.f;
		if( ImGui::Begin("Profiling", pbLinkedBool, ImVec2(ImGui::GetIO().DisplaySize.x/2.f, 0), 0.8f, ImGuiWindowFlags_NoCollapse) )
		{
			zInt sDepth(0);
			zUInt uItemCount(0);			
			ImGui::Text(" ");
			ImGui::Columns(4, nullptr, false);
			ImGui::PushStyleColor(ImGuiCol_Header,			ImVec4(0.1f, 0.25f, 0.1f, 0.9f));
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered,	ImVec4(0.1f, 0.25f, 0.1f, 0.9f));
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
			UIRenderEventTree(rEvent, rEvent->GetElapsedMs(), rEvent->GetElapsedMs(), uItemCount);
			ImGui::End();
		}
		ImGui::GetStyle() = PreviousStyle;
	}	
}

//! @todo perf remove column, use spacing instead, to improve performances
void GfxWindow::UIRenderStatsHistogram(const char* _zHistoID, const char* _zTitle, const zArrayStatic<zenPerf::zEventRef>& _aEvents, const zVec3F& _vColor, float _fWidthRatio)
{
	zArrayStatic<float>	aFrameMs;
	zcRes::GfxWindowRef rWindow		= this;
	const zUInt uFrameIndex			= rWindow->GetFrameCount();
	zUInt uStatCount				= 0;
	float fAvgFrameMs				= 0;
				
	aFrameMs.SetCount( muEventValidCount );
	for(zUInt idx(0); idx<muEventValidCount; ++idx)
	{
		const zUInt uHistoryIndex	= (muEventValidIndex+idx) % keEventHistoryCount;
		float frameMs				= _aEvents[uHistoryIndex].IsValid() ? _aEvents[uHistoryIndex]->GetElapsedMs() : 0.f;
		if( frameMs > 0 )
		{
			aFrameMs[uStatCount++]	= frameMs;
			fAvgFrameMs				+= frameMs;
		}
	}

	if( uStatCount )
	{
		char zFpsTitle[64];
		fAvgFrameMs = fAvgFrameMs/float(uStatCount);
		sprintf(zFpsTitle, _zTitle, fAvgFrameMs, 1000.f / fAvgFrameMs);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.00f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(_vColor.r, _vColor.g, _vColor.b, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(_vColor.r, _vColor.g, _vColor.b, 0.5f));
		ImGui::SameLine();
		ImGui::PlotHistogram(_zHistoID, aFrameMs.First(), uStatCount, 0, zFpsTitle, 0.0f, fAvgFrameMs*2.f, ImVec2( ImGui::GetWindowContentRegionWidth() * _fWidthRatio, 50));
		ImGui::PopStyleColor(3);
		
		if( ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left) )
		{			
			ImVec2 vImMouse			= ImGui::GetMousePos();
			const ImGuiStyle& style = ImGui::GetStyle();
			float minX				= ImGui::GetItemRectMin().x + style.FramePadding.x;
			float maxX				= ImGui::GetItemRectMax().x - style.FramePadding.x;
			float ratio				= (vImMouse.x - minX) / (maxX - minX);
			if( ratio > 0.f && ratio < 1.f )
			{
				zUInt uEventIndex		= int(ratio*uStatCount);
				zUInt uHistoryIndex		= (uEventIndex+muEventValidIndex) % uStatCount;
				mbUIEventProfilingShow	= true;
				mrEventProfiling		= _aEvents[uHistoryIndex];
			}
		}
	}
}

void GfxWindow::UIRenderEventTree(const zenPerf::zEventRef& _rProfilEvent, double _fTotalTime, double _fParentTime, zUInt& _uItemCount, zUInt _uDepth, bool _bGroupedSibbling)
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
			if(rEvent->GetName() == _rProfilEvent->GetName() && !rEvent.GetFirstChild().IsValid())
				return;		
			rEvent = rEvent.GetPrev();
		}
	
		// See if we're first event of many with same parent (to group them)
		fSibblingTime	= _rProfilEvent->GetElapsedMs();
		rEvent			= _rProfilEvent.GetNext();
		while( rEvent.IsValid() )
		{		
			if(rEvent->GetName() == _rProfilEvent->GetName() && !rEvent.GetFirstChild().IsValid() )
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

	bool bOpenNode = UIRenderEventTreeItem(_rProfilEvent->GetName().mzName, uSibblingCount > 0 ? fSibblingTime : _rProfilEvent->GetElapsedMs(), _fTotalTime, _fParentTime, _uItemCount, uChildCount + uSibblingCount, _uDepth);
	++_uItemCount;

	float fTimeUnscoped = _rProfilEvent->GetElapsedMs()-fTimeChilds;
	if( bOpenNode && fTimeUnscoped > 0.01f)
	{
		UIRenderEventTreeItem("Untagged", fTimeUnscoped, _fTotalTime, _rProfilEvent->GetElapsedMs(), _uItemCount, 0, _uDepth+1);
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
				if(rEventSibbling->GetName() == _rProfilEvent->GetName() && !rEventSibbling.GetFirstChild().IsValid() )
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
}

bool GfxWindow::UIRenderEventTreeItem(const zString& _zEventName, double _fEventTime, double _fTotalTime, double _fParentTime, zUInt _uItemCount, zUInt _uChildCount, zUInt _uDepth)
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


}