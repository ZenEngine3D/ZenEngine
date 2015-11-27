namespace zcGfx
{

void Drawcall::ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip )
{
	mrRenderPass				= _rRenderPass;
	mSortId.mSortKeyHi			= 0;
	mSortId.mSortKeyLo			= 0;

	mSortId.muRenderPassID		= _rRenderPass->GetResData()->muPassPriority;
	mSortId.muShaderBindingID	= 0;//! @todo finish this
	mSortId.muGPUPipelineMode	= Drawcall::keGpuPipe_VertexPixel;
	mSortId.mbCullingFrontface	= false;
	mSortId.mbCullingBackface	= true;
	mSortId.mfPriority			= _fPriority;
	mSortId.muRasterStateID		= 0;//mrRenderPass->mrProxRasterState->mpOwner->muCreatedIndex;	//!< @todo clean multihread safe
	mSortId.VertexShaderID		= 0;//! @todo finish this
	mSortId.InputShaderID		= 0;//! @todo finish this

	ZENAssertMsg(mSortId.muRenderPassID == _rRenderPass->GetResData()->muPassPriority, "Not enought bits to support id range" );
//	ZENAssertMsg(mSortId.muRasterStateID == mrRenderPass->mrProxRasterState->mpOwner->muCreatedIndex, "Not enought bits to support id range");
}

void Drawcall::ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, Drawcall::eGPUPipelineMode _eGPUPipelineMode )
{
	mrRenderPass				= _rRenderPass;
	mSortId.mSortKeyHi			= 0;
	mSortId.mSortKeyLo			= 0;
	mSortId.muRenderPassID		= _rRenderPass->GetResData()->muPassPriority;
	mSortId.muShaderBindingID	= _uShaderBindingID; //! @todo urgent assign proper ShaderBindingID
	mSortId.muGPUPipelineMode	= _eGPUPipelineMode;
	mSortId.mbCullingFrontface	= false;
	mSortId.mbCullingBackface	= true;
	mSortId.mfPriority			= _fPriority;
	ZENAssertMsg( mSortId.muRenderPassID == _rRenderPass->GetResData()->muPassPriority, "Not enought bits to support id range" );
}

}
