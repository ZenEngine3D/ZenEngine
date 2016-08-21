#include <math.h>

namespace zen { namespace zenMath 
{
    Matrix& Matrix::SetRight(V4 _vRight){ mvRight = _vRight;return *this;	}
	Matrix& Matrix::SetUp(V4 _vUp)	    { mvUp = _vUp;      return *this;	}	
	Matrix& Matrix::SetAt(V4 _vAt)      { mvAt = _vAt;      return *this;	}
	Matrix& Matrix::SetPos(V4 _vPos)	{ mvPos = _vPos;    return *this;   }
    V4 Matrix::GetRight( void ) const   { return mvRight; }
    V4 Matrix::GetUp( void ) const      { return mvUp; }
    V4 Matrix::GetAt( void ) const      { return mvAt; }
    V4 Matrix::GetPos( void ) const     { return mvPos; }
    

	Matrix& Matrix::operator=(const Matrix& _Copy)
	{
		mvRows[0] = _Copy.mvRows[0];
		mvRows[1] = _Copy.mvRows[1];
		mvRows[2] = _Copy.mvRows[2];
		mvRows[3] = _Copy.mvRows[3];
		return *this;
	}

	Matrix& Matrix::SetIdentity()
	{
		*this = sIdentity;
		return *this;
	}

    Matrix& Matrix::Translate(V4 _vMove)
    {
        Matrix matTranslate( v4X1, v4Y1, v4Z1, _vMove );
        *this *= matTranslate;
        return *this;
    }
    
	Matrix& Matrix::SetRotationX(float _fRadian)
    {
		float fSinAngle = sinf(_fRadian);
		float fCosAngle = cosf(_fRadian);
		mvRows[0]	= simdXYZW( 1, 0, 0, 0);
		mvRows[1]	= simdXYZW( 0, fCosAngle, fSinAngle, 0);
		mvRows[2]	= simdXYZW( 0, -fSinAngle, fCosAngle, 0);
		return *this;     
    }

    Matrix& Matrix::SetRotationY(float _fRadian)
    {
		float fSinAngle = sinf(_fRadian);
		float fCosAngle = cosf(_fRadian);
		mvRows[0]	= simdXYZW( fCosAngle, 0, -fSinAngle, 0);
		mvRows[1]	= simdXYZW( 0, 1, 0, 0);
		mvRows[2]	= simdXYZW( fSinAngle, 0, fCosAngle, 0);
		return *this;     
    }
	
	Matrix& Matrix::SetRotationZ(float _fRadian)
	{
		float fSinAngle = sinf(_fRadian);
		float fCosAngle = cosf(_fRadian);
		mvRows[0]	= simdXYZW( fCosAngle, fSinAngle, 0, 0);
		mvRows[1]	= simdXYZW( -fSinAngle, fCosAngle, 0, 0);
		mvRows[2]	= simdXYZW( 0, 0, 1, 0);
		return *this;  
	}
    
	Matrix& Matrix::Transpose( )
    {		
		V4 T0		= _mm_unpacklo_ps(mvRows[0], mvRows[2]);
		V4 T1		= _mm_unpacklo_ps(mvRows[1], mvRows[3]);
		V4 T2		= _mm_unpackhi_ps(mvRows[0], mvRows[2]);
		V4 T3		= _mm_unpackhi_ps(mvRows[1], mvRows[3]);
		mvRows[0]	= _mm_unpacklo_ps(T0, T1);
		mvRows[1]	= _mm_unpackhi_ps(T0, T1);
		mvRows[2]	= _mm_unpacklo_ps(T2, T3);
		mvRows[3]	= _mm_unpackhi_ps(T2, T3);	
		return *this;
    }

	Matrix& Matrix::operator*=(const Matrix& _TransfoAdd)
	{   
        V4 vCol;
        vCol        = simdMul(  simdSpreadX(mvRows[0]), _TransfoAdd.mvRows[0]);
        vCol        = simdMAdd( simdSpreadY(mvRows[0]), _TransfoAdd.mvRows[1], vCol);
        vCol        = simdMAdd( simdSpreadZ(mvRows[0]), _TransfoAdd.mvRows[2], vCol);
        mvRows[0]   = simdMAdd( simdSpreadW(mvRows[0]), _TransfoAdd.mvRows[3], vCol);

        vCol        = simdMul(  simdSpreadX(mvRows[1]), _TransfoAdd.mvRows[0]);
        vCol        = simdMAdd( simdSpreadY(mvRows[1]), _TransfoAdd.mvRows[1], vCol);
        vCol        = simdMAdd( simdSpreadZ(mvRows[1]), _TransfoAdd.mvRows[2], vCol);
        mvRows[1]   = simdMAdd( simdSpreadW(mvRows[1]), _TransfoAdd.mvRows[3], vCol);

        vCol        = simdMul(  simdSpreadX(mvRows[2]), _TransfoAdd.mvRows[0]);
        vCol        = simdMAdd( simdSpreadY(mvRows[2]), _TransfoAdd.mvRows[1], vCol);
        vCol        = simdMAdd( simdSpreadZ(mvRows[2]), _TransfoAdd.mvRows[2], vCol);
        mvRows[2]   = simdMAdd( simdSpreadW(mvRows[2]), _TransfoAdd.mvRows[3], vCol);

        vCol        = simdMul(  simdSpreadX(mvRows[3]), _TransfoAdd.mvRows[0]);
        vCol        = simdMAdd( simdSpreadY(mvRows[3]), _TransfoAdd.mvRows[1], vCol);
        vCol        = simdMAdd( simdSpreadZ(mvRows[3]), _TransfoAdd.mvRows[2], vCol);
        mvRows[3]   = simdMAdd( simdSpreadW(mvRows[3]), _TransfoAdd.mvRows[3], vCol);

		return *this;
	}

    V4 operator*(V4 _Vect, const Matrix& _Transfo)
    {
        V4 vResult;
        vResult	=   simdMul(  simdSpreadX(_Vect), _Transfo.GetRight() );
        vResult =   simdMAdd( simdSpreadY(_Vect), _Transfo.GetUp(), vResult);
        vResult =   simdMAdd( simdSpreadZ(_Vect), _Transfo.GetAt(), vResult);
        return      simdMAdd( simdSpreadW(_Vect), _Transfo.GetPos(), vResult);
    }

	//=================================================================================================
	//! @brief		Create a view matrix
	//! @details	Taken from Microsoft xna.
	//-------------------------------------------------------------------------------------------------
	//! @param[out] _matViewOut - Result View matrix
	//! @param _vEyePos			- Where we're looking from (camera origin)
	//! @param _vEyeDir			- Look direction
	//! @param _vUpDir			- Up position of camera (in worldspace)	
	//=================================================================================================
	ZENInline void MatrixLookToLH( Matrix& _matViewOut, V4 _vEyePos, V4 _vEyeDir, V4 _vUpDir )
	{		
		zenAssert( !simdIsNearZero(_vEyeDir) );
		zenAssert( !simdIsNearZero(_vUpDir) );
				
		//@todo Optim: optimize by just masking value		
		_vEyePos				= simdMul(_vEyePos, v4XYZ1);	//Discard w
		_vEyeDir				= simdMul(_vEyeDir, v4XYZ1);	//Discard w
		_vUpDir					= simdMul(_vUpDir, v4XYZ1);		//Discard w
		V4 vEyePosNeg			= simdMul(_vEyePos, v4OneNeg);
		
		_matViewOut.mvAt		= simdNormalize(_vEyeDir);
		_matViewOut.mvRight		= simdNormalize( simdCross( _vUpDir, _matViewOut.mvAt ) );
		_matViewOut.mvUp		= simdCross( _matViewOut.mvAt, _matViewOut.mvRight ); 

		float TransX			= -simdDot(_matViewOut.mvRight, _vEyePos);
		float TransY			= -simdDot(_matViewOut.mvUp, _vEyePos);
		float TransZ			= -simdDot(_matViewOut.mvAt, _vEyePos);
		_matViewOut.mvPos		= zenMath::simdXYZW(TransX, TransY, TransZ, 1);

		//Matrix matTranslation( v4X1, v4Y1, v4Z1, Add( vEyePosNeg, v4W1) );
		//_matViewOut.Transpose();//Invert rotation matrix
		//matTranslation *= _matViewOut;
		//_matViewOut = matTranslation;
		//_matViewOut		= matTranslation.Transpose();
		//_matViewOut *= matTranslation;

		/*
		zaxis = normal(cameraTarget - cameraPosition)
		xaxis = normal(cross(cameraUpVector, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x          0
		xaxis.y           yaxis.y           zaxis.y          0
		xaxis.z           yaxis.z           zaxis.z          0
		-dot(xaxis, cameraPosition)  -dot(yaxis, cameraPosition)  -dot(zaxis, cameraPosition)  1
		*/
		/*
		zaxis = normal(At - Eye)
		xaxis = normal(cross(Up, zaxis))
		yaxis = cross(zaxis, xaxis)

		xaxis.x           yaxis.x           zaxis.x          0
		xaxis.y           yaxis.y           zaxis.y          0
		xaxis.z           yaxis.z           zaxis.z          0
		-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l
		*/
	}

	//=================================================================================================
	//! @brief		Create a view matrix
	//! @details	Taken from Microsoft xna.
	//-------------------------------------------------------------------------------------------------
	//! @param[out] _matViewOut - Result View matrix
	//! @param _vEyePos			- Where we're looking from (camera origin)
	//! @param _vTargetPos		- Where we're looking at (target object )
	//! @param _vUpDir			- Up position of camera (in worldspace)	
	//=================================================================================================
	ZENInline void MatrixLookAtLH( Matrix& _matViewOut, V4 _vEyePos, V4 _vTargetPos, V4 _vUpDir )
	{
		MatrixLookToLH(_matViewOut, _vEyePos, simdSub(_vTargetPos, _vEyePos ), _vUpDir );
	}

	//=================================================================================================
	//! @brief		Create a view matrix
	//! @details	Taken from Microsoft xna.
	//-------------------------------------------------------------------------------------------------
	//! @param[out] _matProjectionOut	- Result projection matrix
	//! @param _fFovDegreeWidth			- Horizontal FieldOfView (in degree)
	//! @param _fAspectRatio			- Height on Width ratio
	//! @param _fNearZ					- Near clipping distance
	//! @param _fFarZ					- Far clipping distance
	//=================================================================================================
	ZENInline void MatrixProjectionLH( Matrix& _matProjectionOut, float _fFovDegreeWidth, float _fAspectRatio, float _fNearZ, float _fFarZ )
	{
		zenAssert( _fFovDegreeWidth > zenConst::kfSmallNumber );
		zenAssert( _fAspectRatio > zenConst::kfSmallNumber );
		zenAssert( _fNearZ > zenConst::kfSmallNumber && _fFarZ > _fNearZ );
		
		float ScaleX				= 1.f / tanf(_fFovDegreeWidth/180.0f*zenMath::kfPI);		
		_matProjectionOut.mvRows[0]	= simdXYZW( ScaleX,		0,						0,									0);
		_matProjectionOut.mvRows[1]	= simdXYZW( 0,			ScaleX/_fAspectRatio,	0,									0);
		_matProjectionOut.mvRows[2]	= simdXYZW( 0,			0,						_fFarZ/(_fFarZ-_fNearZ),			1);
		_matProjectionOut.mvRows[3]	= simdXYZW( 0,			0,						-_fFarZ/(_fFarZ-_fNearZ)*_fNearZ,	0);
	}

	//=================================================================================================
	//! @brief		Create a view matrix
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param[out] _matProjectionOut	- Result projection matrix
	//! @param _vfViewportSize			- Viewport size in pixels
	//! @param _fNearZ					- Near clipping distance
	//! @param _fFarZ					- Far clipping distance
	//=================================================================================================
	void MatrixProjectionOrthoLH(Matrix& _matProjectionOut, zVec2F _vfViewportSize, float _fNearZ, float _fFarZ)
	{
		return MatrixProjectionOrthoLH(_matProjectionOut, _vfViewportSize.x, _vfViewportSize.y, _fNearZ, _fFarZ);
	}

	//=================================================================================================
	//! @brief		Create a view matrix
	//! @details	
	//-------------------------------------------------------------------------------------------------
	//! @param[out] _matProjectionOut	- Result projection matrix
	//! @param _fWidth					- Viewport width in pixels
	//! @param _fHeight					- Viewport height in pixels
	//! @param _fNearZ					- Near clipping distance
	//! @param _fFarZ					- Far clipping distance
	//=================================================================================================
	void MatrixProjectionOrthoLH(Matrix& _matProjectionOut, float _fWidth, float _fHeight, float _fNearZ, float _fFarZ)
	{
		zenAssert(_fWidth > zenConst::kfSmallNumber);
		zenAssert(_fHeight > zenConst::kfSmallNumber);
		zenAssert(_fFarZ > _fNearZ);		
		
		_matProjectionOut.mvRows[0] = zenMath::simdXYZW( 2.0f/_fWidth,	 0.0f,			 0.0f,						0.0f);
		_matProjectionOut.mvRows[1] = zenMath::simdXYZW( 0.0f,			-2.0f/_fHeight,  0.0f,						0.0f);
		_matProjectionOut.mvRows[2] = zenMath::simdXYZW( 0.0f,			 0.0f,			 1.0f/(_fFarZ-_fNearZ),		0.0f);
		_matProjectionOut.mvRows[3] = zenMath::simdXYZW(-1.0f,			 1.0f,			_fNearZ/(_fFarZ - _fNearZ), 1.0f);
	}

}  } // namespace zen, zenMath

