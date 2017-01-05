namespace zcGfx
{

const zcRes::GfxRenderPassRef& DrawContext::GetRenderpass()
{
	return mrRenderpass;
}

void DrawContext::AddCommand( const zEngineRef<zcGfx::Command>& _rCommand )
{
	marDrawcalls.Push(_rCommand);
}

}
