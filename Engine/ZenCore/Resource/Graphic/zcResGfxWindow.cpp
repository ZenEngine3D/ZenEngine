#include "zcCore.h"
#include "Engine/ZenExternal/UI/zxUIImgui.h"
#include "Engine/ZenEngine/ToDel/zeWndViewport.h"

namespace zcRes
{

GfxWindow::GfxWindow()
{
	mrImGuiData = new(static_cast<zenMem::zAllocator*>(nullptr), 16) zxImGui::zxRenderData; //! @todo Urgent auto find alignement needs...
}

void GfxWindow::SetBackbuffer(zU8 _uBackbufferIndex, const GfxRenderTargetRef& _rBackbufferColor) 
{ 
	ZENAssert(_uBackbufferIndex < ZENArrayCount(mrBackbufferColor));
	mrBackbufferColor[_uBackbufferIndex] = _rBackbufferColor;
}

const GfxRenderTargetRef& GfxWindow::GetBackbuffer() 
{		
	return mrBackbufferColor[ zcMgr::GfxRender.GetFrameCount() % ZENArrayCount(mrBackbufferColor) ];
}

void GfxWindow::FrameBegin()
{
	zcMgr::GfxRender.FrameBegin(this);
}

void GfxWindow::FrameEnd()
{
	GfxWindow* pCurrentWindow = zcMgr::GfxRender.GetWindowCurrent().Get();
	ZENAssertMsg( this == pCurrentWindow, "Ending frame with different window than started");
	
	//! @todo clean temp debugging	
	WindowInputState InputData;
	mpMainWindowOS->GetInput(InputData, 8); //! @todo urgent cleanup this messy access

	mrImGuiData->mrRendertarget = GetBackbuffer();
	zxImGui::zxImGUIHelper::Get().Render(mrImGuiData, &InputData);
	zcMgr::GfxRender.FrameEnd();
}

zenSig::zSignalEmitter0& GfxWindow::GetSignalUIRender()
{
	ZENAssert(mrImGuiData.IsValid());
	return mrImGuiData->msigRenderUI;
}


}