#include "zeEngine.h"
#include <Engine/ThirdParty/imgui/imgui.h> //! @todo Urgent remove this
namespace zen { namespace zenSys {	

static zEngineInstance*		gpActiveEngine(NULL);

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
	return FSys::EngineStart();
}

void zEngineInstance::Destroy()
{
	mrMainWindowGfx = nullptr;
	zenDelNull(mpMainWindowOS);		
	FSys::EngineStop();	
	gpActiveEngine = NULL;
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
	//! @todo clean temp debugging
	WindowInputState InputData;	
	ImGuiIO& io			= ImGui::GetIO();	
	io.DisplaySize		= ImVec2(mrMainWindowGfx.GetBackbuffer().GetDim().x, mrMainWindowGfx.GetBackbuffer().GetDim().y);	
	io.ImeWindowHandle	= mpMainWindowOS->GetHandle();
	
	mpMainWindowOS->GetInput(InputData, ZENArrayCount(io.InputCharacters) - 1);
	io.MousePos.x		= (zI16)InputData.mvMousePos.x;
	io.MousePos.y		= (zI16)InputData.mvMousePos.y;
	io.MouseWheel		= InputData.mfMouseWheel;
	io.KeyCtrl			= InputData.mbIsKeyDown[VK_CONTROL];
	io.KeyShift			= InputData.mbIsKeyDown[VK_SHIFT]; 
	io.KeyAlt			= InputData.mbIsKeyDown[VK_MENU];

	for(zUInt idx(0); idx<WindowInputState::keMouseBtn__Count; ++idx)
		io.MouseDown[idx] = InputData.mbIsMouseDown[idx];

	for(zUInt idx(0), count(zenMath::Min(InputData.mbIsKeyDown.size(), ZENArrayCount(io.KeysDown))); idx<count; ++idx)
		io.KeysDown[idx] = InputData.mbIsKeyDown[idx];

	for(zUInt idx(0), count(InputData.maCharacterPressed.Count()); idx<count; ++idx)
		if (InputData.maCharacterPressed[idx] > 0 && InputData.maCharacterPressed[idx] < 0x10000)
			io.AddInputCharacter(InputData.maCharacterPressed[idx]);	
}

void zEngineInstance::CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos)
{
	ZENAssert(gpActiveEngine==NULL);
	mpMainWindowOS	= zenNewDefault FWnd::Window(L"MainWindow", _vDim);
	mpMainWindowOS->Initialize();
	mrMainWindowGfx = zenRes::zGfxWindow::Create( mpMainWindowOS->GetHandle() );
	muWindowSize	= _vDim;
}

zSampleEngineInstance::zSampleEngineInstance( void (*_pFunctionToCall)() )
: mpFunctionToCall(_pFunctionToCall)
, mbDone(false)
{
}

bool zSampleEngineInstance::IsDone()
{
	return mbDone;
}
	
void zSampleEngineInstance::Update()
{
	mbDone = false;
	mpFunctionToCall();
	mbDone = true;
}

void zEngineInstance::TempUpdateUIFps()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Framerate", NULL, ImVec2(mrMainWindowGfx.GetBackbuffer().GetDim().x, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);

	//ImGui::Begin("Framerate");
	//ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	bool pause(false);
	//ImGui::BeginGroup();
	//ImGui::Text("Lines");
	//ImGui::PlotLines("##Lines", arr, ZENArrayCount(arr), 0, "Framerate", -1.0f, 1.0f, ImVec2(0, 80));
	//(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, 
	//float scale_min, float scale_max, ImVec2 graph_size, int stride)
	//ImGui::Checkbox("pause", &pause);	

	const zArrayStatic<float>&	aFrameMs	= zcMgr::GfxRender.GetFrameTimeHistory();
	float fFrameAvg						= zcMgr::GfxRender.GetFrameTimeAvg();
	char zTemp[64];
	sprintf(zTemp, "Framerate: %.1fms", fFrameAvg);
	ImGui::PlotHistogram("##Framerate", aFrameMs.First(), aFrameMs.Count(), (zcMgr::GfxRender.GetFrameCount() + 1) % aFrameMs.Count(), zTemp, 0.0f, fFrameAvg*2.f, ImVec2(ImGui::GetContentRegionAvailWidth(), 50));
	//(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text,
	// float scale_min, float scale_max, ImVec2 graph_size, int stride)
	//ImGui::EndGroup();
	ImGui::End();
}

}} // namespace zen, zenRes
