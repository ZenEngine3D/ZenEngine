#pragma once
#ifndef __zenCore_ResourceGfxShaderParam_h__
#define __zenCore_ResourceGfxShaderParam_h__

namespace zen { namespace zenRes {	

	//=============================================================================================
	//! @class	Used for shader compiling preprocessor
	//=============================================================================================
	class awShaderDefine
	{
	ZENClassDeclareNoParent(awShaderDefine)
	public:
		inline		awShaderDefine();
		inline		awShaderDefine(const char* _zName, const char* _zValue);
		inline		awShaderDefine(const zenString& _zName, const zenString& _zValue);
		zenString	mzName;
		zenString	mzValue;
	};

	//=============================================================================================
	//! @class	Used when setting the properties of a texture/sampler pair
	//=============================================================================================
	class awShaderTexture
	{
	ZENClassDeclareNoParent(awShaderTexture)
	public:
		inline awShaderTexture();
		inline awShaderTexture(zenHash32 _hTextureName, zenResID _TextureID, zenResID _SamplerID);

		zenHash32		mhTextureName;
		zenResID	mTextureID;
		zenResID	mSamplerID;
	};

	//=============================================================================================
	//! @class	Base Class used to configure shader parameters. Useful when creating an array
	//!			of various parameters types that can set at once. Child class configure the content
	//!			of this parent
	//=============================================================================================
	class awShaderParameter
	{
	ZENClassDeclareNoParent(awShaderParameter)
	public:
		zenHash32	mhName;						//!< Parameter name
		zenU32		meType			: 4;		//!< eShaderElementType
		zenU32		muVectorSize	: 4;		//!< Number of elements in vector
		zenU32		muArrayCount	: 10;		//!< Number of elements in array
		zenU32		muSizeTotal		: 16;		//!< Total size of the child class
		inline const void* GetData()const {return this + 1;}

	protected:
		inline			awShaderParameter(zenHash32 _hName, awconst::eShaderElementType _eType, zenU16 _uVectorSize, zenU16 _uArrayCount, zenU16 _uSizeTotal);
	};

	template <class TType, awconst::eShaderElementType TShaderType>
	class awShaderParamVector1 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector1, awShaderParameter)
	public:
		inline		awShaderParamVector1( zenHash32 _hName, TType _Value);
		TType		mValue;
	};

	template <class TType, awconst::eShaderElementType TShaderType>
	class awShaderParamVector2 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector2, awShaderParameter)
	public:
		inline		awShaderParamVector2( zenHash32 _hName, TType _Value1, TType _Value2); 
		inline		awShaderParamVector2( zenHash32 _hName, const TType _Values[2]);
		inline		awShaderParamVector2( zenHash32 _hName, const zenVector2<TType>& _vValue);
		TType		mValue[2];
	};

	template <class TType, awconst::eShaderElementType TShaderType>
	class awShaderParamVector3 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector3, awShaderParameter)
	public:
		inline		awShaderParamVector3( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3); 
		inline		awShaderParamVector3( zenHash32 _hName, const TType _Values[3]);
		inline		awShaderParamVector3( zenHash32 _hName, const zenVector3<TType>& _vValue);
		TType		mValue[3];
	};

	template <class TType, awconst::eShaderElementType TShaderType>
	class awShaderParamVector4 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector4, awShaderParameter)
	public:
		inline		awShaderParamVector4( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4);
		inline		awShaderParamVector4( zenHash32 _hName, const TType _Values[4]);
		inline		awShaderParamVector4( zenHash32 _hName, const zenVector4<TType>& _vValues);
		TType		mValue[4];
	};

	typedef awShaderParamVector1<float, awconst::keShaderElemType_Float> awShaderFloat1;
	typedef awShaderParamVector2<float, awconst::keShaderElemType_Float> awShaderFloat2;
	typedef awShaderParamVector3<float, awconst::keShaderElemType_Float> awShaderFloat3;
	typedef awShaderParamVector4<float, awconst::keShaderElemType_Float> awShaderFloat4;

}} // namespace zen, zenRes

#include "zenResGfxShaderParam.inl"

#endif

