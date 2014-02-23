#include "libZenEngine.h"

namespace zen { namespace awsys {	

static awEngineInstance*	gpActiveEngine(NULL);
static FWnd::Window*		gpMainWindow;			//! @todo wrap this inside EngineInstance
void LaunchEngine(awEngineInstance* _pEngineInstance, int argc, const char* const* argv)
{
	AWAssert(!gpActiveEngine);
	gpActiveEngine = _pEngineInstance;
	if( gpActiveEngine->Init() )
	{
		while( !gpActiveEngine->IsDone() && CSys::IsSystemActive() )
		{
			gpActiveEngine->Update();
		}
	}	
	gpActiveEngine->Destroy();
}

awEngineInstance* GetEngineInstance()
{
	return gpActiveEngine;
}

bool awEngineInstance::Init()
{
	return FSys::EngineStart();
}

void awEngineInstance::Destroy()
{
	FSys::EngineStop();
	gpActiveEngine = NULL;
}

void awEngineInstance::Update()
{
	CMgr::Job.Update();
}

void awEngineInstance::CreateGfxWindow(const zenVec2U16& _vDim, const zenVec2U16& _vPos)
{
	AWAssert(gpActiveEngine);
	gpMainWindow	= AWNewDefault FWnd::Window(L"MainWindow", _vDim);
	gpMainWindow->Initialize();
	mrMainGfxWindow = awres::awGfxWindow::Create( gpMainWindow->GetHandle() );
}

void awEngineInstance::SetWindow(const awres::awGfxWindow& _rGfxWindow)
{
	mrMainGfxWindow = _rGfxWindow;
}

}} // namespace zen, awres
