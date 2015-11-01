#pragma once
#ifndef __zCore_Res_Resource_DX11_h__
#define __zCore_Res_Resource_DX11_h__

namespace zcRes
{	
	typedef class GfxSamplerProxy_DX11								GfxSamplerProxy;
	typedef zcExp::ExporterGfxSamplerDX11_DX11						GfxSamplerExporterRuntime;
	typedef zcExp::ExporterGfxSamplerDX11_DX11::ResData				GfxSamplerResData;
	
	typedef class GfxStateBlendProxy_DX11							GfxStateBlendProxy;
	typedef zcExp::ExporterGfxStateBlendDX11_DX11					GfxStateBlendExporterRuntime;
	typedef zcExp::ExporterGfxStateBlendDX11_DX11::ResData			GfxStateBlendResData;
	
	typedef class GfxStateDepthStencilProxy_DX11					GfxStateDepthStencilProxy;
	typedef zcExp::ExporterGfxStateDepthStencilDX11_DX11			GfxStateDepthStencilExporterRuntime;
	typedef zcExp::ExporterGfxStateDepthStencilDX11_DX11::ResData	GfxStateDepthStencilResData;
	
	typedef class GfxStateRasterizerProxy_DX11						GfxStateRasterizerProxy;
	typedef zcExp::ExporterGfxStateRasterizerDX11_DX11				GfxStateRasterizerExporterRuntime;
	typedef zcExp::ExporterGfxStateRasterizerDX11_DX11::ResData		GfxStateRasterizerResData;
	
	typedef class GfxIndexProxy_DX11								GfxIndexProxy;
	typedef zcExp::ExporterGfxIndexDX11_DX11						GfxIndexExporterRuntime;
	typedef zcExp::ExporterGfxIndexDX11_DX11::ResData				GfxIndexResData;
	
	typedef class GfxVertexProxy_DX11								GfxVertexProxy;
	typedef zcExp::ExporterGfxVertexDX11_DX11						GfxVertexExporterRuntime;
	typedef zcExp::ExporterGfxVertexDX11_DX11::ResData				GfxVertexResData;
	
	typedef class GfxInputSignatureProxy_DX11						GfxInputSignatureProxy;
	typedef zcExp::ExporterGfxInputSignatureDX11_DX11				GfxInputSignatureExporterRuntime;
	typedef zcExp::ExporterGfxInputSignatureDX11_DX11::ResData		GfxInputSignatureResData;

	typedef class GfxTexture2dProxy_DX11							GfxTexture2dProxy;
	typedef zcExp::ExporterGfxTexture2dDX11_DX11					GfxTexture2dExporterRuntime;
	typedef zcExp::ExporterGfxTexture2dDX11_DX11::ResData			GfxTexture2dResData;
	
	typedef class GfxRenderTargetProxy_DX11							GfxRenderTargetProxy;
	typedef zcExp::ExporterGfxRenderTarget							GfxRenderTargetExporterRuntime;
	typedef zcExp::ExporterGfxRenderTarget::ResData					GfxRenderTargetResData;

	typedef class GfxViewProxy_DX11									GfxViewProxy;
	typedef zcExp::ExporterGfxView									GfxViewExporterRuntime;
	typedef zcExp::ExporterGfxView::ResData							GfxViewResData;

	typedef class GfxRenderPassProxy_DX11							GfxRenderPassProxy;
	typedef zcExp::ExporterGfxRenderPass_DX11						GfxRenderPassExporterRuntime;
	typedef zcExp::ExporterGfxRenderPass_DX11::ResData				GfxRenderPassResData;

	typedef class GfxWindowProxy_DX11								GfxWindowProxy;
	typedef zcExp::ResDataGfxWindowDX11								GfxWindowResData;

	typedef class GfxInputStreamProxy_DX11							GfxInputStreamProxy;
	typedef zcExp::ExporterGfxInputStreamDX11_DX11					GfxInputStreamExporterRuntime;
	typedef zcExp::ExporterGfxInputStreamDX11_DX11::ResData			GfxInputStreamResData;

	typedef class GfxInputSignatureProxy_DX11						GfxInputSignatureProxy;
	typedef zcExp::ExporterGfxInputSignatureDX11_DX11				GfxInputSignatureExporterRuntime;
	typedef zcExp::ExporterGfxInputSignatureDX11_DX11::ResData		GfxInputSignatureResData;

	typedef class GfxMeshStripProxy_DX11							GfxMeshStripProxy;
	typedef zcExp::ExporterGfxMeshStripDX11_DX11					GfxMeshStripExporterRuntime;
	typedef zcExp::ExporterGfxMeshStripDX11_DX11::ResData			GfxMeshStripResData;

	typedef class GfxMeshProxy_DX11									GfxMeshProxy;
	typedef zcExp::ExporterGfxMeshDX11_DX11							GfxMeshExporterRuntime;
	typedef zcExp::ExporterGfxMeshDX11_DX11::ResData				GfxMeshResData;

	typedef class GfxShaderParamDefProxy_DX11						GfxShaderParamDefProxy;
	typedef zcExp::ExporterGfxShaderParamDefDX11_DX11				GfxShaderParamDefExporterRuntime;
	typedef zcExp::ExporterGfxShaderParamDefDX11_DX11::ResData		GfxShaderParamDefResData;

	typedef class GfxShaderParamProxy_DX11							GfxShaderParamProxy;
	typedef zcExp::ExporterGfxShaderParamDX11_DX11					GfxShaderParamExporterRuntime;		
	typedef zcExp::ExporterGfxShaderParamDX11_DX11::ResData			GfxShaderParamResData;	

	typedef class GfxShaderBindingProxy_DX11						GfxShaderBindingProxy;
	typedef zcExp::ExporterGfxShaderBindingDX11_DX11				GfxShaderBindingExporterRuntime;		
	typedef zcExp::ExporterGfxShaderBindingDX11_DX11::ResData		GfxShaderBindingResData;
	
	typedef class GfxShaderVertexProxy_DX11							GfxShaderVertexProxy;
	typedef class GfxShaderPixelProxy_DX11							GfxShaderPixelProxy;
	typedef zcExp::ExporterGfxShaderDX11_DX11						GfxShaderExporterRuntime;
	typedef zcExp::ExporterGfxShaderDX11_DX11::ResData				GfxShaderResData;	
}

#endif
