#include "zcCore.h"
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace zcGfx 
{

QueryDisjoint_DX11::List	QueryDisjoint_DX11::slstQueryCreated;
QueryTimestamp_DX11::List	QueryTimestamp_DX11::slstQueryCreated;

QueryDisjoint_DX11::QueryDisjoint_DX11()
{
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP_DISJOINT;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.GetDevice()->CreateQuery(&QueryDesc, &mpDX11Query);
	zenAssert(result == S_OK);
	slstQueryCreated.push_front(*this);
}

void QueryDisjoint_DX11::ReferenceDeleteCB()
{
	// This prevents DX warning about starting a new query(when query reused), without having retrieved the value first
	if( !mbValidResult )
		zcMgr::GfxRender.GetDeviceContext()->GetData(mpDX11Query, &mDisjointInfo, sizeof(mDisjointInfo), 0);

	slstQueryCreated.push_front(*this);
}

void QueryDisjoint_DX11::Start(ID3D11DeviceContext* _pContext)
{	
	zenDbgCode(mpDeviceContext=_pContext);

	muFrameStop				= zUInt(-1);
	mbValidResult			= false;
	mDisjointInfo.Disjoint	= true;
	_pContext->Begin(mpDX11Query);	
}

void QueryDisjoint_DX11::Stop(ID3D11DeviceContext* _pContext)
{
	if( muFrameStop )
	{
		zenAssertMsg(mpDeviceContext==_pContext, "Gpu Query started and stopped with different DeviceContext");
		muFrameStop	= zcMgr::GfxRender.GetFrameRendered();
		_pContext->End(mpDX11Query);
	}
}

zU64 QueryDisjoint_DX11::GetClockRate()
{
	zenAssertMsg(muFrameStop != zUInt(-1), "Query need to be started and stopped before we get results back");
	zenAssertMsg(muFrameStop < zcMgr::GfxRender.GetFrameRendered(), "Must wait a complete frame before getting results");
	if( !mbValidResult ) 
	{		
		HRESULT result	= zcMgr::GfxRender.GetDeviceContext()->GetData(mpDX11Query, &mDisjointInfo, sizeof(mDisjointInfo), 0);
		mbValidResult	= (result == S_OK);
	}
	return mDisjointInfo.Disjoint ? 0 : mDisjointInfo.Frequency;
}

zEngineRef<QueryDisjoint_DX11> QueryDisjoint_DX11::Create()
{	
	const zUInt uGrowSize = 8;
	if( slstQueryCreated.empty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenMem::NewPool<QueryDisjoint_DX11>();
	}
	return slstQueryCreated.pop_back();
}

QueryTimestamp_DX11::QueryTimestamp_DX11()
{
	D3D11_QUERY_DESC QueryDesc;
	QueryDesc.Query		= D3D11_QUERY_TIMESTAMP;
	QueryDesc.MiscFlags	= 0;
	HRESULT result		= zcMgr::GfxRender.GetDevice()->CreateQuery(&QueryDesc, &mpDX11Query);
	zenAssert(result == S_OK);
	slstQueryCreated.push_front(*this);
}

void QueryTimestamp_DX11::ReferenceDeleteCB()
{
	// This prevents DX warning about starting a new query(when query reused), without having retrieved the value first
	if( !mbValidResult )
		zcMgr::GfxRender.GetDeviceContext()->GetData(mpDX11Query, &muTimestamp, sizeof(muTimestamp), 0);
	
	mrQueryDisjoint	= nullptr;
	slstQueryCreated.push_front(*this);
}

zU64 QueryTimestamp_DX11::GetTimestampUSec()
{	
	if( mbValidResult == false )
	{
		auto result = zcMgr::GfxRender.GetDeviceContext()->GetData(mpDX11Query, &muTimestamp, sizeof(muTimestamp), 0);
		if( result == S_OK && mrQueryDisjoint.IsValid() )
		{
			zU64 uClockRate = mrQueryDisjoint->GetClockRate();
			if( uClockRate != 0 )
			{
				muTimestamp		= uClockRate ? (muTimestamp*1000*1000/uClockRate) : 0;
				mbValidResult	= true;
				mrQueryDisjoint = nullptr;			
			}
		}
	}
	return muTimestamp;
}

zEngineRef<QueryTimestamp_DX11> QueryTimestamp_DX11::Create()
{	
	const zUInt uGrowSize = 256;
	if( slstQueryCreated.empty() )
	{
		for(zUInt idx(0); idx<uGrowSize; ++idx)
			zenMem::NewPool<QueryTimestamp_DX11>();
	}

	QueryTimestamp_DX11* pQuery	= slstQueryCreated.pop_back();
	pQuery->mrQueryDisjoint		= zcMgr::GfxRender.GetQueryDisjoint();
	pQuery->mbValidResult		= false;
	pQuery->muTimestamp			= 0;
	return pQuery;
}

bool ManagerRender_DX11::Load()
{	
	HRESULT hr	= S_OK;
	
	//-------------------------------------------------------------------------
	// Configure Texture Format mapping
	//-------------------------------------------------------------------------
	zenMem::Set(meFormatConversion, DXGI_FORMAT_UNKNOWN, sizeof(meFormatConversion) );	
	meFormatConversion[zenConst::keTexFormat_R8]		= DXGI_FORMAT_R8_UNORM;	
	meFormatConversion[zenConst::keTexFormat_RGB8]		= DXGI_FORMAT_UNKNOWN;
	meFormatConversion[zenConst::keTexFormat_RGBA8]		= DXGI_FORMAT_R8G8B8A8_UNORM;
	meFormatConversion[zenConst::keTexFormat_D24S8]		= DXGI_FORMAT_D24_UNORM_S8_UINT;
	meFormatConversion[zenConst::keTexFormat_D32]		= DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	meFormatConversion[zenConst::keTexFormat_BC1]		= DXGI_FORMAT_BC1_UNORM;	
	meFormatConversion[zenConst::keTexFormat_BC2]		= DXGI_FORMAT_BC2_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC3]		= DXGI_FORMAT_BC3_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC5]		= DXGI_FORMAT_BC5_UNORM;
	meFormatConversion[zenConst::keTexFormat_BC7]		= DXGI_FORMAT_BC7_UNORM;
	meFormatConversion[zenConst::keTexFormat_RGBA32f]	= DXGI_FORMAT_R32_FLOAT;

	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	const zArrayFixed<D3D_FEATURE_LEVEL,1> aFeatureLevels = { D3D_FEATURE_LEVEL_11_0 };
	mDX11DriverType	= D3D_DRIVER_TYPE_HARDWARE;	//Only support hardware dx11 support

	hr =  D3D11CreateDevice(
		nullptr, 
		mDX11DriverType,	
		nullptr, 
		createDeviceFlags, 
		aFeatureLevels.Data(),
		(UINT)aFeatureLevels.size(),
		D3D11_SDK_VERSION, 
		&mDX11pDevice, 
		&mDX11FeatureLevel, 
		&mDX11pContextImmediate
		);

	if( FAILED( hr ) )
		return FALSE;

	D3D11_INPUT_ELEMENT_DESC EmptyDesc;
	hr = mDX11pContextImmediate->QueryInterface( __uuidof(mDX11pPerf), reinterpret_cast<void**>(&mDX11pPerf) );	
	if( FAILED( hr ) )
		return FALSE;

	//----------------------------------------------------------------------------------------------
	// Create an empty input layout since not using them.
	//----------------------------------------------------------------------------------------------
	// We're binding struct buffer directly instead, more generic and flexible, 
	// This has no impact on performances
	//! @todo Store empty shader blob instead?
	const char zEmptyVertexShaderCode[] = "void main(uint VertexId : SV_VertexID){}";
	ID3DBlob *pErrorBlob(nullptr), *pShaderCompiled(nullptr);
	if( SUCCEEDED(D3DCompile(zEmptyVertexShaderCode, zenArrayCount(zEmptyVertexShaderCode), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &pShaderCompiled, &pErrorBlob) ) )
	{
		hr = mDX11pDevice->CreateInputLayout( &EmptyDesc, 0, pShaderCompiled->GetBufferPointer(), pShaderCompiled->GetBufferSize(), &mDX11pEmptyInputLayout );
	}
	else if( pErrorBlob )
	{
		printf("\nError Compiling shader inputlayout shader :\n%s\n", (char*)pErrorBlob->GetBufferPointer() );
	}
		
	if( pShaderCompiled )	
		pShaderCompiled->Release();
	if( pErrorBlob )
		pErrorBlob->Release();

	if( FAILED( hr ) )
		return FALSE;

	return TRUE;
}

bool ManagerRender_DX11::Unload()
{
	if( mDX11pPerf ) 
		mDX11pPerf->Release();

	if( mDX11pContextImmediate )
	{	
		mDX11pContextImmediate->ClearState();
		mDX11pContextImmediate->Release();
	}
	if( mDX11pDevice )				
		mDX11pDevice->Release();

	mDX11pPerf				= nullptr;
	mDX11pContextImmediate	= nullptr;
	mDX11pDevice			= nullptr;
	
	return true;
}

void ManagerRender_DX11::FrameBegin(zcRes::GfxWindowRef _FrameWindow)
{
	mDX11pContextImmediate->IASetInputLayout( mDX11pEmptyInputLayout );
	mGpuContext[0].Reset(mDX11pDevice, mDX11pContextImmediate);
	Super::FrameBegin(_FrameWindow);	

	mbDX11ProfilerDetected	= mDX11pPerf && mDX11pPerf->GetStatus();
	mrQueryDisjoint			= QueryDisjoint_DX11::Create();
	mrQueryDisjoint->Start(mDX11pContextImmediate);	
}

void ManagerRender_DX11::FrameEnd()
{			
	if( mrFrameContext.IsValid() )
	{
		mrFrameContext.Submit();
		mrFrameContext = nullptr;
	}
	
	mrQueryDisjoint->Stop(mDX11pContextImmediate);		
	grWindowRender.HAL()->mpDX11SwapChain->Present( 0, 0 );	
	UnbindResources();
	Super::FrameEnd();
}

void ManagerRender_DX11::NamedEventBegin(const char* _zName)
{
	if( mbDX11ProfilerDetected )
	{
		WCHAR zEventName[64];
		mbstowcs_s(nullptr, zEventName, _zName, zenArrayCount(zEventName));
		mDX11pPerf->BeginEvent(zEventName);
	}
}

void ManagerRender_DX11::NamedEventEnd()
{
	if( mbDX11ProfilerDetected )
		mDX11pPerf->EndEvent();
}

const zEngineRef<QueryDisjoint_DX11>& ManagerRender_DX11::GetQueryDisjoint()const
{
	return mrQueryDisjoint;
}

void ManagerRender_DX11::SubmitToGPU(const CommandListRef& _rCommandlist, const zArrayDyn<CommandRef>& _rCommands)
{
	zenAssert(_rCommands.empty()==false);
	// Potential Resolve RT -> Texture should be here...
	const zEngineRef<zcGfx::Command>* prDrawcall	= _rCommands.Data(); //SF iterator instead
	const zEngineRef<zcGfx::Command>* prDrawcallEnd	= &_rCommands.back();	
	while( prDrawcall <= prDrawcallEnd )
	{
		(*prDrawcall)->Invoke(mGpuContext[0]);
		++prDrawcall;		
	}
}

}