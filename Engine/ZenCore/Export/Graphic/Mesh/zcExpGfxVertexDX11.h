#pragma once
#if !defined(__zCore_Exp_Gfx_SerialGfxVertex_DX11_h__) && ZEN_EXPORT_OR_RESOURCE_DX11
#define __zCore_Exp_Gfx_SerialGfxVertex_DX11_h__

namespace zcExp
{
	class ExportGfxVertexDX11 : public ExportGfxVertex
	{
	zenClassDeclare(ExportGfxVertexDX11, ExportGfxVertex)
	public:
		zArrayStatic<D3D11_INPUT_ELEMENT_DESC>	maElementDef;	//!< D3D11 infos on each vertice element contained in all buffer (pos, uv, etc...)	
	};
}

#endif
