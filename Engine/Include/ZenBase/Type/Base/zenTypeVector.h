#pragma once
#ifndef __LibCore_Api_Type_Vector_h__
#define __LibCore_Api_Type_Vector_h__

namespace zen { namespace awtype {
	template<typename TType>
	class awVector2
	{
	AWClassDeclareNoParent(awVector2)
	public:
								awVector2();
								awVector2(TType _Val);
								awVector2(TType aX, TType aY );
								awVector2(const awVector2& _vCopy);
		inline awVector2&		operator=(const awVector2& _vCopy);
		inline awVector2&		operator=(TType _Val);		
		inline awVector2		operator*(TType _Val)const;
		inline awVector2		operator+(const awVector2& _vAdd)const;
		inline bool				operator==(const awVector2& _vCompare)const;
		inline bool				operator!=(const awVector2& _vCompare)const;		
		inline bool				IsNull();
		inline const awVector2& SetNull();
		union
		{
			struct {TType x; TType y;};
			struct {TType u; TType v;};
			TType xy[2];
		};
		static const awVector2 Zero;
		static const awVector2 One;
	};

	template<typename TType>
	class awVector3
	{
	AWClassDeclareNoParent(awVector3)
	public:
								awVector3();
								awVector3(TType _val);
								awVector3(TType aX, TType aY, TType aZ );
								awVector3(const awVector3& _vCopy);
		inline awVector3&		operator=(const awVector3& _vCopy);
		inline awVector3&		operator=(TType _Val);
		inline awVector3		operator*(TType _Val)const;
		inline awVector3		operator+(const awVector3& _vAdd)const;
		inline bool				operator==(const awVector3& _vCompare)const;
		inline bool				operator!=(const awVector3& _vCompare)const;
		inline bool				IsNull();
		inline const awVector3& SetNull();
		union
		{
			struct {TType x; TType y; TType z;};
			struct {TType u; TType v; TType s;};
			struct {TType r; TType g; TType b;};
			TType xyz[3];
		};
		static const awVector3 Zero;
		static const awVector3 One;
	};
	
	template<typename TType>
	class awVector4
	{
	AWClassDeclareNoParent(awVector4)
	public:
								awVector4();
								awVector4(TType _Val);
								awVector4(TType aX, TType aY, TType aZ, TType aW );
								awVector4(const awVector4& _vCopy);
		inline awVector4&		operator=(const awVector4& _vCopy);
		inline awVector4&		operator=(TType _Val);
		inline awVector4		operator*(TType _Val)const;
		inline awVector4		operator+(const awVector4& _vAdd)const;
		inline bool				operator==(const awVector4& _vCompare)const;
		inline bool				operator!=(const awVector4& _vCompare)const;
		inline bool				IsNull();
		inline const awVector4& SetNull();
		union
		{
			struct {TType x; TType y; TType z; TType w;};
			struct {TType u; TType v; TType s; TType t;};
			struct {TType r; TType g; TType b; TType a;};
			TType xyzw[4];
		};
		static const awVector4 Zero;
		static const awVector4 One;
	};

	typedef awVector2<awS16>	awVec2S16;	AWAddSupportsMemCopy(awVec2S16);
	typedef awVector2<awS32>	awVec2S32;	AWAddSupportsMemCopy(awVec2S32);
	typedef awVector2<awU16>	awVec2U16;	AWAddSupportsMemCopy(awVec2U16);
	typedef awVector2<awU32>	awVec2U32;	AWAddSupportsMemCopy(awVec2U32);
	typedef awVector2<awF32>	awVec2F;	AWAddSupportsMemCopy(awVec2F);
	typedef awVector3<awS16>	awVec3S16;	AWAddSupportsMemCopy(awVec3S16);
	typedef awVector3<awS32>	awVec3S32;	AWAddSupportsMemCopy(awVec3S32);
	typedef awVector3<awU16>	awVec3U16;	AWAddSupportsMemCopy(awVec3U16);
	typedef awVector3<awU32>	awVec3U32;	AWAddSupportsMemCopy(awVec3U32);
	typedef awVector3<awF32>	awVec3F;	AWAddSupportsMemCopy(awVec3F);
	typedef awVector4<awS16>	awVec4S16;	AWAddSupportsMemCopy(awVec4S16);
	typedef awVector4<awS32>	awVec4S32;	AWAddSupportsMemCopy(awVec4S32);
	typedef awVector4<awU16>	awVec4U16;	AWAddSupportsMemCopy(awVec4U16);
	typedef awVector4<awU32>	awVec4U32;	AWAddSupportsMemCopy(awVec4U32);
	typedef awVector4<awF32>	awVec4F;	AWAddSupportsMemCopy(awVec4F);
}} // namespace zen, awtype

#include "zenTypeVector.inl"

#endif
