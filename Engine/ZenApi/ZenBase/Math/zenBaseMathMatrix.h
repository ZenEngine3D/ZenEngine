#pragma once
#ifndef __zenApi_Base_Math_Matrix_h__
#define __zenApi_Base_Math_Matrix_h__

namespace zen { namespace zenMath 
{
	class alignas(16) Matrix
	{
	zenClassDeclareNoParent(Matrix);
	public:
							Matrix		(){};
							Matrix		(const Matrix& _Copy);
							Matrix		(const V4& _vRight, const V4& _vUp, const V4& _vAt, const V4& _vPos );

		zenInline Matrix&	SetIdentity	( );
		zenInline Matrix&	SetRight	(V4 _vRight);
		zenInline Matrix&	SetUp		(V4 _vUp);
		zenInline Matrix&	SetAt		(V4 _vAt);
		zenInline Matrix&	SetPos		(V4 _vPos);

		zenInline V4		GetRight	( ) const;
		zenInline V4		GetUp		( ) const;
		zenInline V4		GetAt		( ) const;
		zenInline V4		GetPos		( ) const;

		zenInline Matrix&	Translate		(V4 _vMove);
		zenInline Matrix&	SetRotationX	(float _fRadian);
		zenInline Matrix&	SetRotationY	(float _fRadian);
		zenInline Matrix&	SetRotationZ	(float _fRadian);

		zenInline Matrix&	Transpose	( );
		zenInline Matrix&	operator=	(const Matrix& _Copy);
		zenInline Matrix&	operator*=  (const Matrix& _TransfoAdd);

		static const Matrix sIdentity;

	protected:
		union
		{
			struct
			{
				V4 mvRows[4];
			};
			struct
			{
				V4 mvRight;
				V4 mvUp;
				V4 mvAt;
				V4 mvPos;
			};
		};
	};

	zenInline V4 operator*(V4 _Vect, const Matrix& _Transfo);
	zenInline void MatrixLookAtLH( V4 _vEyePos, V4 _vTargetPos, V4 _vUpDir, Matrix& _matViewOut );
	zenInline void MatrixLookToLH( V4 _vEyePos, V4 _vEyeDir, V4 _vUpDir, Matrix& _matViewOut );
	zenInline void MatrixProjectionLH( float _fFovDegreeWidth, float _fAspectRatio, float _fNearZ, float _fFarZ, Matrix& _matProjectionOut );
	zenInline void MatrixProjectionOrthoLH(Matrix& _matProjectionOut, zVec2F _vfViewportSize, float _fNearZ, float _fFarZ);
	zenInline void MatrixProjectionOrthoLH(Matrix& _matProjectionOut, float _fWidth, float _fHeight, float _fNearZ, float _fFarZ);
}  } // namespace zen, zenMath
	
#include "zenBaseMathMatrix.inl"

#endif
