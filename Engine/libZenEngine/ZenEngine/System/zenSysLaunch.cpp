#include "libZenEngine.h"

namespace zen { namespace awsys {	

static awEngineInstance*	gpActiveEngine(NULL);
static FWnd::Window*		gpMainWindow;			//! @todo wrap this inside EngineInstance
void LaunchEngine(awEngineInstance* _pEngineInstance, int argc, const char* const* argv)
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

void SetEngine(awEngineInstance* _pEngineInstance)
{
	ZENAssert(!gpActiveEngine);
	gpActiveEngine = _pEngineInstance;
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
	ZENAssert(gpActiveEngine==NULL);
	gpMainWindow	= zenNewDefault FWnd::Window(L"MainWindow", _vDim);
	gpMainWindow->Initialize();
	mrMainGfxWindow = zenRes::GfxWindow::Create( gpMainWindow->GetHandle() );
}

void awEngineInstance::SetWindow(const zenRes::GfxWindow& _rGfxWindow)
{
	mrMainGfxWindow = _rGfxWindow;
}

}} // namespace zen, zenRes
