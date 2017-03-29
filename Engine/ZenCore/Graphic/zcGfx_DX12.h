#pragma once

#include <d3d12.h>
#include <DXGI1_4.h>
#include <wrl.h>
#include "ZenExternal/DX12/d3dx12.h"


template<class TResouce>
using DirectXComRef = Microsoft::WRL::ComPtr<TResouce>;

//! @todo 2 Generalize this to all rendering api
zenInline void zSetGfxResourceName( const DirectXComRef<ID3D12Resource>& _rResource, const LPCWSTR _zName )
{
#if !BUILD_FINAL
	_rResource->SetName(_zName);
#endif
}

zenInline void zSetGfxResourceName( const DirectXComRef<ID3D12Resource>& _rResource, zResID _ResID, const LPCWSTR _zName )
{
#if !BUILD_FINAL
	WCHAR DebugName[128];
	const WCHAR* pzResTypeDesc[] = {
		L"ShaderPixel",
		L"ShaderVertex",
		L"CBufferDef",
		L"CBuffer",
		L"ShaderBind",
		L"Target2D",
		L"View",
		L"Window",
		L"Index",
		L"Buffer",
		L"Texture2D",
		L"Sampler",
		L"Blend",
		L"DepthStencil",
		L"Raster",
		L"MeshStrip",
		L"Mesh",
		L"RenderPass",
		L"Invalid"
	};
	zenStaticAssert( zenArrayCount(pzResTypeDesc)==keResType__Count+1 );
	if( swprintf_s(DebugName, L"%s [Id:0x%08X, %s, %s]",	_zName ? _zName : L"Unnamed", 															
															(const zU32&)_ResID.GetName(), 
															pzResTypeDesc[_ResID.GetType()],
															_ResID.GetSource() == keResSource_Runtime ? L"Dyn" : L"Load" ) > 0 )
	{
		zSetGfxResourceName(_rResource, DebugName);
	}
#endif
}

// Graphcis files specific to DirectX12
#include <ZenCore/Graphic/DX12/zcGfxRootSignature_DX12.h>
#include <ZenCore/Graphic/DX12/zcGfxPso_DX12.h>
#include <zenCore/Graphic/DX12/zcGfxResourceDescriptor_DX12.h>
#include <ZenCore/Graphic/DX12/zcGfxGpuBuffer_DX12.h>