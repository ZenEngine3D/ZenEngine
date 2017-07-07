#pragma once

namespace zcRes
{
	class GfxTexture2D_DX11 : public zcExp::ExportGfxTexture2D
	{
	public:
		virtual											~GfxTexture2D_DX11();
		bool											Initialize();
	
		ID3D11Texture2D*								mpTextureBuffer;
		ID3D11ShaderResourceView*						mpTextureView;
		typedef zcExp::ExporterGfxTexture2DDX11_DX11	RuntimeExporter;
	};
}
