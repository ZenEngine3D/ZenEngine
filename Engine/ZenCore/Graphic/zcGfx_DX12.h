#pragma once
#ifndef __zCore_Gfx_Base_DX12_h__
#define __zCore_Gfx_Base_DX12_h__ 

#include <d3d12.h>
#include "ZenExternal/DX12/d3dx12.h"
#include <wrl.h>
#include <DXGI1_4.h>

template<class TResouce>
using DirectXComRef = Microsoft::WRL::ComPtr<TResouce>;

#include <ZenCore/Graphic/zcGfxRootSignature_DX12.h>

#endif
