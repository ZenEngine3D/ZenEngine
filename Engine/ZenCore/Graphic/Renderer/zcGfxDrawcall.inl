namespace zcGfx
{

void Drawcall::ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, const zcRes::GfxMeshStripRef& _rMeshStrip )
{
	mrRenderPass				= _rRenderPass->GetProxy();
	mSortId.muRenderPassID		= _rRenderPass.GetResID().GetName();
	mSortId.muShaderBindingID	= 0;//_rMeshStrip->; //! @todo urgent assign proper VertexId (need refactor Resource/proxy)
	mSortId.muGPUPipelineMode	= Drawcall::keGpuPipe_VertexPixel;
	mSortId.mbCullingFrontface	= false;
	mSortId.mbCullingBackface	= true;
	mSortId.mfPriority			= _fPriority;
	ZENAssertMsg( mSortId.muRenderPassID == _rRenderPass.GetResID().GetName(), "Not enought bits to support id range" );
}

void Drawcall::ConfigureBase( const zcRes::GfxRenderPassRef& _rRenderPass, float _fPriority, zU32 _uShaderBindingID, Drawcall::eGPUPipelineMode _eGPUPipelineMode )
{
	mrRenderPass				= _rRenderPass->GetProxy();
	mSortId.muRenderPassID		= _rRenderPass.GetResID().GetName();
	mSortId.muShaderBindingID	= _uShaderBindingID; //! @todo urgent assign proper ShaderBindingID
	mSortId.muGPUPipelineMode	= _eGPUPipelineMode;
	mSortId.mbCullingFrontface	= false;
	mSortId.mbCullingBackface	= true;
	mSortId.mfPriority			= _fPriority;
	ZENAssertMsg( mSortId.muRenderPassID == _rRenderPass.GetResID().GetName(), "Not enought bits to support id range" );
}

}
