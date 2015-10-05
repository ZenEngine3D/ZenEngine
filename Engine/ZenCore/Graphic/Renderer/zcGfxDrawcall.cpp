#include "zcCore.h"

namespace zcGfx
{

zEngineRef<Drawcall> Drawcall::Create( const zcRes::GfxRenderPassRef& _rRenderPass, const zcRes::GfxMeshStripRef& _rMeshStrip,	const float& _fPriority )
{
	static zenMem::zAllocatorPool sMemPool("Pool Drawcall", sizeof(Drawcall), 2048, 2048 );
	Drawcall* pDrawcall						= zenNew(&sMemPool) Drawcall;	
	pDrawcall->mrRenderPass					= _rRenderPass->GetProxy();
	pDrawcall->mrMeshStrip					= _rMeshStrip->GetProxy();
	pDrawcall->mSortId.muRenderPassID		= _rRenderPass.GetResID().GetName();
	pDrawcall->mSortId.muShaderBindingID	= 0; //! @todo Urgent
	pDrawcall->mSortId.muShaderMode			= 0; //! @todo Urgent feature Support compute/tesselation/vertex only...
	pDrawcall->mSortId.mfPriority			= _fPriority;

	ZENAssertMsg( pDrawcall->mSortId.muRenderPassID == _rRenderPass.GetResID().GetName(), "Not enought bits to support id range" );
	
	return pDrawcall;
}

}
