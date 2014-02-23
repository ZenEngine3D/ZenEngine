#pragma once
#ifndef __zenBase_Type_Vector_h__
#define __zenBase_Type_Vector_h__

namespace zen { namespace zenType {
	template<typename TType>
	class zenVector2
	{
	AWClassDeclareNoParent(zenVector2)
	public:
								zenVector2();
								zenVector2(TType _Val);
								zenVector2(TType aX, TType aY );
								zenVector2(const zenVector2& _vCopy);
		inline zenVector2&		operator=(const zenVector2& _vCopy);
		inline zenVector2&		operator=(TType _Val);		
		inline zenVector2		operator*(TType _Val)const;
		inline zenVector2		operator+(const zenVector2& _vAdd)const;
		inline bool				operator==(const zenVector2& _vCompare)const;
		inline bool				operator!=(const zenVector2& _vCompare)const;		
		inline bool				IsNull();
		inline const zenVector2& SetNull();
		union
		{
			struct {TType x; TType y;};
			struct {TType u; TType v;};
			TType xy[2];
		};
		static const zenVector2 Zero;
		static const zenVector2 One;
	};

	template<typename TType>
	class zenVector3
	{
	AWClassDeclareNoParent(zenVector3)
	public:
								zenVector3();
								zenVector3(TType _val);
								zenVector3(TType aX, TType aY, TType aZ );
								zenVector3(const zenVector3& _vCopy);
		inline zenVector3&		operator=(const zenVector3& _vCopy);
		inline zenVector3&		operator=(TType _Val);
		inline zenVector3		operator*(TType _Val)const;
		inline zenVector3		operator+(const zenVector3& _vAdd)const;
		inline bool				operator==(const zenVector3& _vCompare)const;
		inline bool				operator!=(const zenVector3& _vCompare)const;
		inline bool				IsNull();
		inline const zenVector3& SetNull();
		union
		{
			struct {TType x; TType y; TType z;};
			struct {TType u; TType v; TType s;};
			struct {TType r; TType g; TType b;};
			TType xyz[3];
		};
		static const zenVector3 Zero;
		static const zenVector3 One;
	};
	
	template<typename TType>
	class zenVector4
	{
	AWClassDeclareNoParent(zenVector4)
	public:
								zenVector4();
								zenVector4(TType _Val);
								zenVector4(TType aX, TType aY, TType aZ, TType aW );
								zenVector4(const zenVector4& _vCopy);
		inline zenVector4&		operator=(const zenVector4& _vCopy);
		inline zenVector4&		operator=(TType _Val);
		inline zenVector4		operator*(TType _Val)const;
		inline zenVector4		operator+(const zenVector4& _vAdd)const;
		inline bool				operator==(const zenVector4& _vCompare)const;
		inline bool				operator!=(const zenVector4& _vCompare)const;
		inline bool				IsNull();
		inline const zenVector4& SetNull();
		union
		{
			struct {TType x; TType y; TType z; TType w;};
			struct {TType u; TType v; TType s; TType t;};
			struct {TType r; TType g; TType b; TType a;};
			TType xyzw[4];
		};
		static const zenVector4 Zero;
		static const zenVector4 One;
	};

	typedef zenVector2<zenS16>	zenVec2S16;	AWAddSupportsMemCopy(zenVec2S16);
	typedef zenVector2<zenS32>	zenVec2S32;	AWAddSupportsMemCopy(zenVec2S32);
	typedef zenVector2<zenU16>	zenVec2U16;	AWAddSupportsMemCopy(zenVec2U16);
	typedef zenVector2<zenU32>	zenVec2U32;	AWAddSupportsMemCopy(zenVec2U32);
	typedef zenVector2<zenF32>	zenVec2F;	AWAddSupportsMemCopy(zenVec2F);
	typedef zenVector3<zenS16>	zenVec3S16;	AWAddSupportsMemCopy(zenVec3S16);
	typedef zenVector3<zenS32>	zenVec3S32;	AWAddSupportsMemCopy(zenVec3S32);
	typedef zenVector3<zenU16>	zenVec3U16;	AWAddSupportsMemCopy(zenVec3U16);
	typedef zenVector3<zenU32>	zenVec3U32;	AWAddSupportsMemCopy(zenVec3U32);
	typedef zenVector3<zenF32>	zenVec3F;	AWAddSupportsMemCopy(zenVec3F);
	typedef zenVector4<zenS16>	zenVec4S16;	AWAddSupportsMemCopy(zenVec4S16);
	typedef zenVector4<zenS32>	zenVec4S32;	AWAddSupportsMemCopy(zenVec4S32);
	typedef zenVector4<zenU16>	zenVec4U16;	AWAddSupportsMemCopy(zenVec4U16);
	typedef zenVector4<zenU32>	zenVec4U32;	AWAddSupportsMemCopy(zenVec4U32);
	typedef zenVector4<zenF32>	zenVec4F;	AWAddSupportsMemCopy(zenVec4F);
}} // namespace zen, zenType

#include "zenTypeVector.inl"

#endif
