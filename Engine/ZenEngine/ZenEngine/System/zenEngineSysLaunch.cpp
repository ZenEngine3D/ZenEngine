#include "zeEngine.h"

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
	zVec2U16 vWindowSize = mpMainWindowOS->GetSize();
	if( muWindowSize != vWindowSize )
	{
		muWindowSize = vWindowSize;
		mrMainWindowGfx.Resize(muWindowSize);
	}
	zcMgr::Updater.Update(zenConst::keUpdt_FrameStart);
	msigUpdate.Emit(zenConst::keUpdt_FrameStart);
	Update(); //! @todo Clean remove this and use signals
	zcMgr::Updater.Update(zenConst::keUpdt_FrameEnd);
	msigUpdate.Emit(zenConst::keUpdt_FrameEnd);
}

void zEngineInstance::Update()
{
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

}} // namespace zen, zenRes
