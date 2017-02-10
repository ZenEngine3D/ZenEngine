namespace zcGfx 
{

const DirectXComRef<ID3D12GraphicsCommandList>& GPUContext_DX12::GetCommandList() const
{
	return mrCommandList; 
}

} 
