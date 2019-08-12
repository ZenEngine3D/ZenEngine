namespace zcGfx
{

const zcRes::GfxRenderPassRef& CommandList_Base::GetRenderpass()
{
	return mrRenderpass;
}

void CommandList_Base::AddCommand( const zEngineRef<zcGfx::Command>& _rCommand )
{	
	marDrawcalls[mbInsertBeforeChilds ? 0 : 1].push_back(_rCommand);
}

void CommandList_Base::SetBeforeChildren(bool _bBeforeChild)
{
	mbInsertBeforeChilds = _bBeforeChild;
}

bool CommandList_Base::IsBeforeChildren()const
{
	return mbInsertBeforeChilds;
}

bool CommandList_Base::IsEmpty() const
{
	return marDrawcalls[0].empty() && marDrawcalls[1].empty() && mlstChilds.empty();
}

}
