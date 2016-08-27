#pragma once
#ifndef __zenApi_Base_Type_Vector_h__
#define __zenApi_Base_Type_Vector_h__

namespace zen { namespace zenType 
{
	template<typename TType>
	class zVector2
	{
	zenClassDeclareNoParent(zVector2)
	public:
		zVector2();
		zVector2(TType _Val);
		zVector2(TType aX, TType aY );
		zVector2(const zVector2& _vCopy);
		ZENInline zVector2&			operator=(const zVector2& _vCopy);
		ZENInline zVector2&			operator=(TType _Val);		
		ZENInline zVector2			operator*(TType _Val)const;
		ZENInline zVector2			operator-(const zVector2& _vAdd)const;
		ZENInline zVector2			operator+(const zVector2& _vAdd)const;
		ZENInline bool				operator==(const zVector2& _vCompare)const;
		ZENInline bool				operator!=(const zVector2& _vCompare)const;		
		ZENInline bool				IsZero();
		ZENInline const zVector2&	SetZero();
		union
		{
			struct {TType x; TType y;};
			struct {TType u; TType v;};
			TType xy[2];
			TType values[2];
		};
		static const zVector2 Zero;
		static const zVector2 One;
	};

	template<typename TType>
	class zVector3
	{
	zenClassDeclareNoParent(zVector3)
	public:
									zVector3();
									zVector3(TType _val);
									zVector3(TType aX, TType aY, TType aZ );
									zVector3(const zVector3& _vCopy);
		ZENInline zVector3&			operator=(const zVector3& _vCopy);
		ZENInline zVector3&			operator=(TType _Val);
		ZENInline zVector3			operator*(TType _Val)const;
		ZENInline zVector3			operator+(const zVector3& _vAdd)const;
		ZENInline bool				operator==(const zVector3& _vCompare)const;
		ZENInline bool				operator!=(const zVector3& _vCompare)const;
		ZENInline bool				IsZero();
		ZENInline const zVector3&	SetZero();
		union
		{
			struct {TType x; TType y; TType z;};
			struct {TType u; TType v; TType s;};
			struct {TType r; TType g; TType b;};
			TType xyz[3];
			TType values[3];
		};
		static const zVector3 Zero;
		static const zVector3 One;
	};
	
	template<typename TType>
	class zVector4
	{
	zenClassDeclareNoParent(zVector4)
	public:
									zVector4();
									zVector4(TType _Val);
									zVector4(TType aX, TType aY, TType aZ, TType aW );
									zVector4(const zVector4& _vCopy);
		ZENInline zVector4&			operator=(const zVector4& _vCopy);
		ZENInline zVector4&			operator=(TType _Val);
		ZENInline zVector4			operator*(TType _Val)const;
		ZENInline zVector4			operator+(const zVector4& _vAdd)const;
		ZENInline bool				operator==(const zVector4& _vCompare)const;
		ZENInline bool				operator!=(const zVector4& _vCompare)const;
		ZENInline bool				IsZero();
		ZENInline const zVector4&	SetZero();
		union
		{
			struct {TType x; TType y; TType z; TType w;};
			struct {TType u; TType v; TType s; TType t;};
			struct {TType r; TType g; TType b; TType a;};
			TType xyzw[4];
			TType values[4];
		};
		static const zVector4 Zero;
		static const zVector4 One;
	};

	typedef zVector2<zI16>	zVec2S16;	
	typedef zVector2<zI32>	zVec2S32;	
	typedef zVector2<zU16>	zVec2U16;	
	typedef zVector2<zU32>	zVec2U32;	
	typedef zVector2<float>	zVec2F;		
	typedef zVector3<zI16>	zVec3S16;	
	typedef zVector3<zI32>	zVec3S32;	
	typedef zVector3<zU16>	zVec3U16;	
	typedef zVector3<zU32>	zVec3U32;	
	typedef zVector3<float>	zVec3F;		
	typedef zVector4<zI16>	zVec4S16;	
	typedef zVector4<zI32>	zVec4S32;	
	typedef zVector4<zU16>	zVec4U16;	
	typedef zVector4<zU32>	zVec4U32;	
	typedef zVector4<float>	zVec4F;		
}} // namespace zen, zenType

#include "zenBaseTypeVector.inl"

#endif
