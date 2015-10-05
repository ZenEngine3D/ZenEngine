#include "zcCore.h"

namespace zen { namespace zenRes {

zGfxSampler zGfxSampler::Create( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
{
	return zcExp::CreateGfxSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

zGfxStateBlend zGfxStateBlend::Create( zenType::zBlendDesc* _pBlendDesc )
{
	return zcExp::CreateGfxBlend(_pBlendDesc);
}

zGfxStateDepthStencil zGfxStateDepthStencil::Create( bool _bDepthEnable, bool _bDepthWrite, bool _bStencilEnable, zU8 _uStencilReadMask, zU8 _uStencilWriteMask, zenConst::eComparisonFunc _eDepthFunc, zenType::zDepthStencilDesc::DepthStencilOp _xFrontFace, zenType::zDepthStencilDesc::DepthStencilOp _xBackFace )
{
	return zcExp::CreateGfxDepthStencil(_bDepthEnable, _bDepthWrite, _bStencilEnable, _uStencilReadMask, _uStencilWriteMask, _eDepthFunc, _xFrontFace, _xBackFace);
}

zGfxStateRasterizer zGfxStateRasterizer::Create( bool _bFrontCounterClockwise, bool _bDepthClipEnable, bool _bScissorEnable, bool _bMultisampleEnable, bool _bAntialiasedLineEnable, bool _bWireFrame, zenConst::eCullMode _eCullMode, zI32 _iDepthBias, float _fDepthBiasClamp, float _fSlopeScaledDepthBias )
{
	return zcExp::CreateGfxRasterizer(_bFrontCounterClockwise, _bDepthClipEnable, _bScissorEnable, _bMultisampleEnable, _bAntialiasedLineEnable, _bWireFrame, _eCullMode, _iDepthBias, _fDepthBiasClamp, _fSlopeScaledDepthBias);
}

}} // namespace zen, zenRes
