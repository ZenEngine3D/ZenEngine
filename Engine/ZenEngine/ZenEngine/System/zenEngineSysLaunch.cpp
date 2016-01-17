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
	zcMgr::Updater.Update(zenConst::keUpdt_FrameStart);
	msigUpdate.Emit(zenConst::keUpdt_FrameStart);
	Update(); //! @todo Clean remove this and use signals
	zcMgr::Updater.Update(zenConst::keUpdt_FrameEnd);
	msigUpdate.Emit(zenConst::keUpdt_FrameEnd);
	zenRes::zResource::ReleaseUnused();
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
	static zI16 suStatsGPUFrame = -1;

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

	ImVec2 vPos = ImVec2(0, ImGui::GetWindowContentRegionMin().y);
	ImGui::SetNextWindowPos(vPos);
	
	ImGuiStyle& CurrentStyle	= ImGui::GetStyle();
	ImGuiStyle PreviousStyle	= CurrentStyle;
	CurrentStyle.WindowPadding	= ImVec2(0, 0);
	CurrentStyle.FrameRounding	= 0.f;
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

	if(sbShowFps && ImGui::Begin("Fps", nullptr, ImVec2(ImGui::GetIO().DisplaySize.x,0), 0.1f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings) )
	{				
		char zFpsGpuTitle[64]; 
		const zArrayStatic<float>&	aFrameMs	= zcMgr::GfxRender.GetFrameTimeHistory();
		float fFrameAvg							= zcMgr::GfxRender.GetFrameTimeAvg();		
		int valOffet							= (zcMgr::GfxRender.GetStatsFrame() + 1) % aFrameMs.Count();
		sprintf(zFpsGpuTitle, "GPU: %.1fms (%.1ffps)", fFrameAvg, 1000.f / fFrameAvg);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.00f, 0.0f));		
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram,			ImVec4(0.2f, 0.8f, 0.2f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered,	ImVec4(0.2f, 0.8f, 0.2f, 0.5f));
		ImGui::PlotHistogram("##FramerateGPU", aFrameMs.First(), aFrameMs.Count(), valOffet, zFpsGpuTitle, 0.0f, fFrameAvg*2.f, ImVec2(ImGui::GetContentRegionAvailWidth()/2.f, 50));
		ImGui::PopStyleColor(2);
		
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left))
		{
			zcMgr::GfxRender.SetStatsUpdate(!zcMgr::GfxRender.GetStatsUpdate());
		}

		char zFpsCpuTitle[64];
		sprintf(zFpsCpuTitle, "CPU: %.1fms (%.1ffps)", fFrameAvg, 1000.f / fFrameAvg);
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram,			ImVec4(0.2f, 0.2f, 0.8f, 0.2f));
		ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered,	ImVec4(0.2f, 0.2f, 0.8f, 0.5f));
		ImGui::PlotHistogram("##FramerateCPU", aFrameMs.First(), aFrameMs.Count(), valOffet, zFpsCpuTitle, 0.0f, fFrameAvg*2.f, ImVec2(ImGui::GetContentRegionAvailWidth(), 50));
		ImGui::PopStyleColor(3);
				
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left) )
		{
			zcMgr::GfxRender.SetStatsUpdate(!zcMgr::GfxRender.GetStatsUpdate());
		}
			
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(WindowInputState::keMouseBtn_Left) )
		{
			ImVec2 vImMin	= ImGui::GetItemRectMin();
			ImVec2 vImMax	= ImGui::GetItemRectMax();
			ImVec2 vImMouse = ImGui::GetMousePos();
			const ImGuiStyle& style = ImGui::GetStyle();
			float minX		= vImMin.x + style.FramePadding.x;
			float maxX		= vImMax.x - style.FramePadding.x;
			float ratio		= (vImMouse.x - minX) / (maxX - minX);
			if( ratio > 0.f && ratio < 1.f )
				suStatsGPUFrame = int(ratio*aFrameMs.Count());
		}
		
		if( suStatsGPUFrame >= 0 )
		{

		}
		//ImGui::EndChild();
		//(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text,
		// float scale_min, float scale_max, ImVec2 graph_size, int stride)
		//ImGui::EndGroup();
		
		ImGui::End();
	}
	ImGui::PopStyleColor();
	CurrentStyle = PreviousStyle;
}


void zEngineInstance::UIRenderStats( )
{	
	//printf("Hovering %i\n", int(ratio*aFrameMs.Count()));	
}

}} // namespace zen, zenRes
