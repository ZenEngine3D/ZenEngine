#include "zeEngine.h"
#include <Engine/ThirdParty/imgui/imgui.h> //! @todo Urgent remove this
namespace zen { namespace zenSys {	

static zEngineInstance*	gpActiveEngine(nullptr);

void LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const* argv)
{	
	if( _pEngineInstance->Init() )
	{
		zenAssert(!gpActiveEngine);
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
	zenAssert(gpActiveEngine);
	return gpActiveEngine;
}

bool zEngineInstance::Init()
{
	return zbSys::EngineStart();
}

void zEngineInstance::Destroy()
{
	//mrMainWindowGfx = nullptr;
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
			mpMainWindowOS->GetGfxWindow().Resize(muWindowSize);
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
			zenPerf::zScopedEventCpu EmitEvent("Refcounted Release");
			zRefCounted::ReferenceRelease();
		}
	}
}

void zEngineInstance::Update()
{	
}

void zEngineInstance::CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos)
{
	zenAssert(gpActiveEngine==nullptr);
	zenAssert(mpMainWindowOS == nullptr);
	//! @todo Clean improve windows creation to be more os agnostic, and more parameters (fullscreen, etc...)
	//! @todo clean make os window a resource too
	mpMainWindowOS					= zenNewDefault zenWnd::Window(L"MainWindow", _vDim);
	mpMainWindowOS->Initialize();
	mrMainWindowGfx					= mpMainWindowOS->GetGfxWindow();
	mrMainWindowGfx->mpMainWindowOS	= mpMainWindowOS;
	muWindowSize					= _vDim;
	mrMainWindowGfx->GetSignalUIRender().Connect(*this, &zEngineInstance::UIRenderCB);
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
			ImGui::MenuItem("ImGUI Demo", nullptr, &mbUIShowImGUITest);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (mbUIShowImGUITest)
		ImGui::ShowTestWindow();
}

void zEngineInstance::UIRenderCB()
{
	UIRender();	
}

void zEngineInstance::UIRender()
{
}


}} // namespace zen, zenRes
