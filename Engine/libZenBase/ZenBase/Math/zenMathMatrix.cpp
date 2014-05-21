#include "libZenBase.h"

namespace zen { namespace zenMath 
{
const Matrix Matrix::sIdentity( simdXYZW(1.0f, 0.0f, 0.0f, 0.0f), 
								simdXYZW(0.0f, 1.0f, 0.0f, 0.0f), 
								simdXYZW(0.0f, 0.0f, 1.0f, 0.0f),
								simdXYZW(0.0f, 0.0f, 0.0f, 1.0f) );


Matrix::Matrix(const Matrix& _Copy)
{
    mvRows[0] = _Copy.mvRows[0];
    mvRows[1] = _Copy.mvRows[1];
    mvRows[2] = _Copy.mvRows[2];
    mvRows[3] = _Copy.mvRows[3];
}

Matrix::Matrix(const V4& _vRight, const V4& _vUp, const V4& _vAt, const V4& _vPos )
{
    mvRight = _vRight;
    mvUp    = _vUp;
    mvAt    = _vAt;
    mvPos   = _vPos;
}
}  } // namespace zen, zenMath

