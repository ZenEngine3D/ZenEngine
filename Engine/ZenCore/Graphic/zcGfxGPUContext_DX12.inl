namespace zcGfx 
{

const DirectXComRef<ID3D12GraphicsCommandList>& GPUContext_DX12::GetCommandList() const
{
	return mrCommandList; 
}

void GPUContext_DX12::AddFence(const DirectXComRef<ID3D12Fence>& _rFence, zU64 mValue)
{
	maPendingFences.push_back( {_rFence, mValue} );
}

} 
