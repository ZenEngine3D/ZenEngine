#include "zcCore.h"

namespace zcMgr 
{ 
	zcGfx::ManagerRender GfxRender; 	
}

namespace zcGfx
{

zcRes::GfxWindowRef grWindowRender;

ManagerRender_Base::ManagerRender_Base()
{
	mrFrameContext	= zenGfx::zCommandList::Create("PreFrame");
}

void ManagerRender_Base::FrameBegin( zcRes::GfxWindowRef _FrameWindow )
{
	zenAssertMsg( !grWindowRender.IsValid(), "End previous frame before starting a new one" );
	grWindowRender		= _FrameWindow;
	
	if( mrFrameContext.IsValid() )
		mrFrameContext.Submit();

	zcGfx::Command::ResetCommandCount();
	mrFrameContext	= zenGfx::zCommandList::Create("PostFrame");
}

void ManagerRender_Base::FrameEnd()
{
	if( mrFrameContext.IsValid() )
		mrFrameContext.Submit();
	mrFrameContext	= zenGfx::zCommandList::Create("PreFrame");

	grWindowRender = nullptr;
	++muFrameRendered;
}

#if 0 //! @todo 2 check if there's anything still of value in here
zenAssertMsg(_Drawlist.mbSubmitted == false, "Can only submit a context once");
	if( !_Drawlist.marDrawcalls.IsEmpty() || !_Drawlist.mlstChilds.IsEmpty() )
	{
		_Drawlist.mbSubmitted		= true;
 		zcPerf::EventBaseRef rEvent = zcPerf::EventGPU::Create(_Drawlist.mzName);
 		rEvent->GPUStart(&_Drawlist);	//Get sorted to start of drawlist
		rEvent->GPUStop(&_Drawlist); 	//Get sorted to end of drawlist (after children commands)
		_Drawlist.marDrawcalls.Sort();

		//------------------------------------------------------------------------------------------
		// Add Barriers for Rendertargets
		bool bBarrierPostUpdate 	= false;
		zcRes::GfxView_HAL* pView	= _Drawlist.GetRenderpass().IsValid() && _Drawlist.GetRenderpass().HAL()->mrStateView.IsValid() ? _Drawlist.GetRenderpass().HAL()->mrStateView.HAL() : nullptr;
		if( pView )
		{		
			zcRes::GfxTexture2DRef rRTTexture;		
			for( zUInt idx(0), count(pView->maRTColorConfig.Count()); idx<count; ++idx)
			{
				rRTTexture = pView->maRTColorConfig[idx].mrTargetSurface.IsValid() && pView->maRTColorConfig[idx].mrTargetSurface->GetTexture2D().IsValid() ? pView->maRTColorConfig[idx].mrTargetSurface->GetTexture2D() : nullptr;
				if( rRTTexture.IsValid() )
					_Drawlist.AddBarrierCheck(true, ScopedDrawlist_DX12::BarrierCheck(&rRTTexture.HAL()->mResource, D3D12_RESOURCE_STATE_RENDER_TARGET));
			}
		
			rRTTexture = pView->mRTDepthConfig.mrTargetSurface.IsValid() ? pView->mRTDepthConfig.mrTargetSurface->GetTexture2D() : nullptr;
			if( rRTTexture.IsValid() )
				_Drawlist.AddBarrierCheck(true, ScopedDrawlist_DX12::BarrierCheck(&rRTTexture.HAL()->mResource, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		}
		DispatchBarrier(_Drawlist, true);

		//------------------------------------------------------------------------------------------
		// Invoke all Pre-Children drawcalls		
		const zEngineRef<zcGfx::Command>* prDrawcall	= _Drawlist.GetCommands().First();
		const zEngineRef<zcGfx::Command>* prDrawcallEnd	= _Drawlist.GetCommands().Last();
		if( !_Drawlist.marDrawcalls.IsEmpty() )
		{
			while( prDrawcall <= prDrawcallEnd && (*prDrawcall)->mSortId.Draw.muGPUPipelineMode <= keGpuPipe__PreChildLast )
			{
				//! @todo 2 optim Find better way of barrier before/after DataUpdate
				if( !bBarrierPostUpdate && (*prDrawcall)->mSortId.Draw.muGPUPipelineMode > keGpuPipe_DataUpdate )
				{			
					DispatchBarrier(_Drawlist, false);
					bBarrierPostUpdate = true;
				}
				(*prDrawcall)->Invoke(mGpuContext[0]);
				++prDrawcall;		
			}
		}		
		if( !bBarrierPostUpdate )
		{			
			DispatchBarrier(_Drawlist, false);
			bBarrierPostUpdate = true;
		}

		//------------------------------------------------------------------------------------------
		// Invoke all Children drawcalls
		ScopedDrawlist* pChildCur = static_cast<ScopedDrawlist*>(_Drawlist.mlstChilds.GetHead());
		while( pChildCur )
		{
			//pChildCur->SubmitInternal();
			Render(*pChildCur);
			pChildCur = static_cast<ScopedDrawlist*>(_Drawlist.mlstChilds.GetNext(*pChildCur));
		}

		//------------------------------------------------------------------------------------------
		// Invoke all Post-Children drawcalls
		if( !_Drawlist.marDrawcalls.IsEmpty() )
		{
			while( prDrawcall <= prDrawcallEnd )
			{
				(*prDrawcall)->Invoke(mGpuContext[0]);
				++prDrawcall;		
			}
		}		
	}
#endif
}
