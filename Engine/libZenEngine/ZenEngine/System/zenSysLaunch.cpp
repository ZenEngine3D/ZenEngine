#include "libZenEngine.h"

namespace zen { namespace zenSys {	

static zEngineInstance*		gpActiveEngine(NULL);
static FWnd::Window*		gpMainWindow;			//! @todo Clean: Wrap this inside EngineInstance
void LaunchEngine(zEngineInstance* _pEngineInstance, int argc, const char* const* argv)
{	
	if( _pEngineInstance->Init() )
	{
		SetEngine(_pEngineInstance);
		while( !gpActiveEngine->IsDone() && CSys::IsSystemActive() )
		{
			gpActiveEngine->Update();
		}
	}	
	gpActiveEngine->Destroy();
}

void SetEngine(zEngineInstance* _pEngineInstance)
{
	ZENAssert(!gpActiveEngine);
	gpActiveEngine = _pEngineInstance;
}

zEngineInstance* GetEngineInstance()
{
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

void zEngineInstance::Update()
{
	CMgr::Job.Update();
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

}} // namespace zen, zenRes
