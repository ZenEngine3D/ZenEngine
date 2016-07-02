namespace zcGfx
{

void Command::SetSortKeyDraw(const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip)
{
	mrRenderPass					= _rRenderPass;
	mSortId.mSortKeyHi				= 0;
	mSortId.mSortKeyLo				= 0;
	mSortId.Draw.muRenderPassID		= _rRenderPass->GetResData()->muPassPriority;
	
	mSortId.Draw.muGPUPipelineMode	= Command::keGpuPipe_VertexPixel;
	mSortId.Draw.muShaderBindingID	= 0;//! @todo finish this
	mSortId.Draw.mbCullingFrontface	= false;
	mSortId.Draw.mbCullingBackface	= true;
	mSortId.Draw.mfPriority			= _fPriority;
	mSortId.Draw.muRasterStateID	= 0;//mrRenderPass->mrProxRasterState->mpOwner->muCreatedIndex;	//!< @todo clean multihread safe
	mSortId.Draw.muVertexShaderID	= 0;//! @todo finish this
	mSortId.Draw.muInputShaderID	= 0;//! @todo finish this

	ZENAssertMsg(mSortId.Draw.muRenderPassID == _rRenderPass->GetResData()->muPassPriority, "Not enough bits to support id range" );
}

void Command::SetSortKeyCompute(const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, bool _bBeforeDraw)
{
	mrRenderPass						= _rRenderPass;
	mSortId.mSortKeyHi					= 0;
	mSortId.mSortKeyLo					= 0;
	mSortId.Compute.muRenderPassID		= _rRenderPass->GetResData()->muPassPriority;

	mSortId.Compute.muGPUPipelineMode	= _bBeforeDraw ? keGpuPipe_PreDrawCompute : keGpuPipe_PostDrawCompute;
	mSortId.Compute.muShaderBindingID	= _uShaderBindingID; //! @todo urgent assign proper ShaderBindingID	
	mSortId.Compute.mfPriority			= _fPriority;
	ZENAssertMsg(mSortId.Compute.muRenderPassID == _rRenderPass->GetResData()->muPassPriority, "Not enough bits to support id range");
}

void Command::SetSortKeyDataUpdate(/*const zcRes::GfxRenderPassRef& _rRenderPass,*/ zU64 _uResID)
{
	mrRenderPass							= nullptr;	//! @todo Urgent remove renderpass from base command, use it only when needed?
	mSortId.mSortKeyHi						= 0;
	mSortId.mSortKeyLo						= 0;
	mSortId.DataUpdate.muRenderPassID		= 0;//_rRenderPass->GetResData()->muPassPriority;
	
	mSortId.DataUpdate.muGPUPipelineMode	= keGpuPipe_DataUpdate;
	mSortId.DataUpdate.muResID				= _uResID;
	
//	ZENAssertMsg(mSortId.DataUpdate.muRenderPassID == _rRenderPass->GetResData()->muPassPriority, "Not enough bits to support id range");
}

}
