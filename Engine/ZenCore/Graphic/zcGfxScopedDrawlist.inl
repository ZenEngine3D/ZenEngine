namespace zcGfx
{

const zcRes::GfxRenderPassRef& ScopedDrawlist_Base::GetRenderpass()
{
	return mrRenderpass;
}

void ScopedDrawlist_Base::AddCommand( const zEngineRef<zcGfx::Command>& _rCommand )
{
	marDrawcalls.Push(_rCommand);
}

}
