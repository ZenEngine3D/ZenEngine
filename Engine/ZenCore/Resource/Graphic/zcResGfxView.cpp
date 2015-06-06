#include "zcCore.h"

namespace zcRes
{

void GfxView::Clear( bool _bClearColor, const zVec4F& _vRGBA, bool _bClearDepth, float _fDepth, bool _bClearStencil, zU8 _uStencil )
{
	mrProxy->Clear(_bClearColor, _vRGBA, _bClearDepth, _fDepth, _bClearStencil, _uStencil);
}

const zVec2S16& GfxView::GetOrigin()const
{
	return mrExportData->mvOrigin;
}

const zVec2U16& GfxView::GetDim()const
{
	return mrExportData->mvDim;
}

}