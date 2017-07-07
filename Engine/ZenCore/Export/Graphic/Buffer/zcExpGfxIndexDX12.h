#pragma once

namespace zcExp
{	

class ExportGfxIndexDX12 : public ExportGfxIndex
{
zenClassDeclare(ExportGfxIndexDX12, ExportGfxIndex)
public:
		
	D3D12_PRIMITIVE_TOPOLOGY		mePrimitive;		//!< Primitive type (TRIANGLELIST, TRIANGLESTRIP, ...)
	D3D12_PRIMITIVE_TOPOLOGY_TYPE	mePrimitiveType;	//!< Triangle, point, etc...
	DXGI_FORMAT						meIndiceFormat;		//!< Indice size (DXGI_FORMAT_R16_UINT,DXGI_FORMAT_R32_UINT, ...)
};

}
