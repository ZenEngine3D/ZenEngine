#include "zcCore.h"

namespace zcRes
{

GfxRenderPassProxy_DX11::GfxRenderPassProxy_DX11()
: mzStageName("Uninitialized")
{
}

GfxRenderPassProxy_DX11::~GfxRenderPassProxy_DX11()
{
}

bool GfxRenderPassProxy_DX11::Initialize(class GfxRenderPass& _Owner)
{
	const GfxRenderPass::ResDataRef& rResData = _Owner.GetResData();
	ZENAssert(rResData.IsValid());
	ZENDbgCode(mpOwner = &_Owner);
			
	mzStageName				= rResData->mzStageName;
	mrProxBlendState		= GetResourceProxy<GfxStateBlendRef>(rResData->mBlendStateID);
	mrProxDepthStencilState	= GetResourceProxy<GfxStateDepthStencilRef>(rResData->mDepthStencilStateID);
	mrProxRasterState		= GetResourceProxy<GfxStateRasterizerRef>(rResData->mRasterStateID);
	mrProxViewState			= GetResourceProxy<GfxViewRef>(rResData->mViewStateID);

	return true;
}

}
