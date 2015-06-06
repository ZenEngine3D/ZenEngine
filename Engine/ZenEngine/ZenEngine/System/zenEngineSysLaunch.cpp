#include "zeEngine.h"

namespace zen { namespace zenSys {	

static zEngineInstance*		gpActiveEngine(NULL);
static FWnd::Window*		gpMainWindow;			//! @todo Clean: Wrap this inside EngineInstance

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
	FSys::EngineStop();
	gpActiveEngine = NULL;
}

void zEngineInstance::MainLoop()
{
	zcMgr::Job.Update();	
	zcMgr::Updater.Update(zenConst::keUpdt_FrameStart);
	msigUpdate.Emit(zenConst::keUpdt_FrameStart);

	zcMgr::Updater.Update(zenConst::keUpdt_FrameEnd);
	msigUpdate.Emit(zenConst::keUpdt_FrameEnd);

	Update(); //! @todo Clean remove this and use signals
}

void zEngineInstance::Update()
{
}

void zEngineInstance::CreateGfxWindow(const zVec2U16& _vDim, const zVec2U16& _vPos)
{
	ZENAssert(gpActiveEngine==NULL);
	gpMainWindow	= zenNewDefault FWnd::Window(L"MainWindow", _vDim);
	gpMainWindow->Initialize();
	mrMainGfxWindow = zenRes::zGfxWindow::Create( gpMainWindow->GetHandle() );
}

void zEngineInstance::SetWindow(const zenRes::zGfxWindow& _rGfxWindow)
{
	mrMainGfxWindow = _rGfxWindow;
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

}} // namespace zen, zenRes
