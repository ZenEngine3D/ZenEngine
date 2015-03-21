#pragma once
#ifndef __zenApi_Base_Math_Matrix_h__
#define __zenApi_Base_Math_Matrix_h__

namespace zen { namespace zenMath 
{
	class Matrix
	{
	ZENClassDeclareNoParent(Matrix);
	public:
							Matrix		(){};
							Matrix		(const Matrix& _Copy);
							Matrix		(const V4& _vRight, const V4& _vUp, const V4& _vAt, const V4& _vPos );

		ZENInline Matrix&		SetIdentity	( );
		ZENInline Matrix&		SetRight	(V4 _vRight);
		ZENInline Matrix&		SetUp		(V4 _vUp);
		ZENInline Matrix&		SetAt		(V4 _vAt);
		ZENInline Matrix&		SetPos		(V4 _vPos);

		ZENInline V4			GetRight	( ) const;
		ZENInline V4			GetUp		( ) const;
		ZENInline V4			GetAt		( ) const;
		ZENInline V4			GetPos		( ) const;

		ZENInline Matrix&		Translate		(V4 _vMove);
		ZENInline Matrix&		SetRotationX	(float _fRadian);
		ZENInline Matrix&		SetRotationY	(float _fRadian);
		ZENInline Matrix&		SetRotationZ	(float _fRadian);

		ZENInline Matrix&		Transpose	( );
		ZENInline Matrix&		operator=	(const Matrix& _Copy);
		ZENInline Matrix&		operator*=  (const Matrix& _TransfoAdd);

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

	ZENInline V4 operator*(V4 _Vect, const Matrix& _Transfo);
	ZENInline void MatrixLookAtLH( V4 _vEyePos, V4 _vTargetPos, V4 _vUpDir, Matrix& _matViewOut );
	ZENInline void MatrixLookToLH( V4 _vEyePos, V4 _vEyeDir, V4 _vUpDir, Matrix& _matViewOut );
	ZENInline void MatrixProjectionLH( float _fFovDegreeWidth, float _fAspectRatio, float _fNearZ, float _fFarZ, Matrix& _matProjectionOut );

}  } // namespace zen, zenMath
	
#include "zenBaseMathMatrix.inl"

#endif
