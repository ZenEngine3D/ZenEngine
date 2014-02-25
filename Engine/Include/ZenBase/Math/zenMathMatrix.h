#pragma once
#ifndef __LibZenBase_Api_Math_Matrix_h__
#define __LibZenBase_Api_Math_Matrix_h__

namespace zen { namespace zenMath 
{
	class Matrix
	{
	AWClassDeclareNoParent(Matrix);
	public:
							Matrix		(){};
							Matrix		(const Matrix& _Copy);
							Matrix		(const V4& _vRight, const V4& _vUp, const V4& _vAt, const V4& _vPos );

		inline Matrix&		SetIdentity	( );
		inline Matrix&		SetRight	(V4 _vRight);
		inline Matrix&		SetUp		(V4 _vUp);
		inline Matrix&		SetAt		(V4 _vAt);
		inline Matrix&		SetPos		(V4 _vPos);

		inline V4			GetRight	( ) const;
		inline V4			GetUp		( ) const;
		inline V4			GetAt		( ) const;
		inline V4			GetPos		( ) const;

		inline Matrix&		Translate		(V4 _vMove);
		inline Matrix&		SetRotationX	(float _fRadian);
		inline Matrix&		SetRotationY	(float _fRadian);
		inline Matrix&		SetRotationZ	(float _fRadian);

		inline Matrix&		Transpose	( );
		inline Matrix&		operator=	(const Matrix& _Copy);
		inline Matrix&		operator*=  (const Matrix& _TransfoAdd);

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

	inline V4 operator*(V4 _Vect, const Matrix& _Transfo);
	inline void MatrixLookAtLH( V4 _vEyePos, V4 _vTargetPos, V4 _vUpDir, Matrix& _matViewOut );
	inline void MatrixLookToLH( V4 _vEyePos, V4 _vEyeDir, V4 _vUpDir, Matrix& _matViewOut );
	inline void MatrixProjectionLH( float _fFovDegreeWidth, float _fAspectRatio, float _fNearZ, float _fFarZ, Matrix& _matProjectionOut );

}  } // namespace zen, zenMath
	
#include "zenMathMatrix.inl"

#endif
