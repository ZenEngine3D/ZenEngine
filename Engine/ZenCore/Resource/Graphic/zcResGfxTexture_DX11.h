#pragma once
#ifndef __zCore_Res_Gfx_Texture_DX11_h__
#define __zCore_Res_Gfx_Texture_DX11_h__

namespace zcRes
{
	class GfxTexture2dHAL_DX11 : public zcExp::ExportGfxTexture2d
	{
	public:
		virtual											~GfxTexture2dHAL_DX11();
		bool											Initialize();
	
		ID3D11Texture2D*								mpTextureBuffer;
		ID3D11ShaderResourceView*						mpTextureView;
		typedef zcExp::ExporterGfxTexture2dDX11_DX11	RuntimeExporter;
	};
	class GfxTexture2dHAL : public GfxTexture2dHAL_DX11{};

}

#endif
