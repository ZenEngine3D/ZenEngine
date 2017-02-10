#include "zcCore.h"

namespace zen { namespace zenRes {

zGfxStateSampler zGfxStateSampler::Create( zenConst::eTextureFiltering _eFilterMin, zenConst::eTextureFiltering _eFilterMag, zenConst::eTextureWrap _eWrapU, zenConst::eTextureWrap _eWrapV, float _fLodBias, const zVec4F& _vBorderColor )
{
	return zcExp::CreateGfxStateSampler(_eFilterMin, _eFilterMag, _eWrapU, _eWrapV, _fLodBias, _vBorderColor);
}

zGfxStateRaster zGfxStateRaster::Create( const zGfxStateRaster::Config& _RasterConfig )
{
	return zcExp::CreateGfxRasterizer(_RasterConfig);
}

}} // namespace zen, zenRes
