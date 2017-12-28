namespace zcGfx
{

void Command::SetSortKeyDraw(const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip)
{	
	mrRenderPass					= _rRenderPass;
	mSortId.mSortKeyHi				= 0;
	mSortId.mSortKeyLo				= 0;
	
	mSortId.Draw.muGPUPipelineMode	= keGpuPipe_VertexPixel;
	mSortId.Draw.muShaderBindingID	= 0;//! @todo finish this
	mSortId.Draw.mbCullingFrontface	= false;
	mSortId.Draw.mbCullingBackface	= true;
	mSortId.Draw.mfPriority			= _fPriority;
	mSortId.Draw.muRasterStateID	= 0;//mrRenderPass->mrProxRasterState->mpOwner->muCreatedIndex;	//!< @todo clean multihread safe
	mSortId.Draw.muVertexShaderID	= 0;//! @todo finish this
	mSortId.Draw.muInputShaderID	= 0;//! @todo finish this
}

void Command::SetSortKeyCompute(const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, bool _bBeforeDraw)
{
	mrRenderPass							= _rRenderPass;
	mSortId.mSortKeyHi						= 0;
	mSortId.mSortKeyLo						= 0;

	mSortId.Compute.muGPUPipelineMode		= _bBeforeDraw ? keGpuPipe_ComputePreDraw : keGpuPipe_ComputePostDraw;
	mSortId.Compute.muShaderBindingID		= _uShaderBindingID; //! @todo urgent assign proper ShaderBindingID	
	mSortId.Compute.mfPriority				= _fPriority;
}

void Command::SetSortKeyGeneric	( eSubmitPhase _eSubmitPhase, zU64 _uSortID )
{
	mrRenderPass							= nullptr;
	mSortId.mSortKeyHi						= 0;
	mSortId.mSortKeyLo						= 0;
	
	mSortId.Generic.muGPUPipelineMode		= _eSubmitPhase;
	mSortId.Generic.muSortID				= _uSortID;
}

}
