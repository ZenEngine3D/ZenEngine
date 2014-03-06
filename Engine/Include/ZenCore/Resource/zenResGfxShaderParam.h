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
		ZENInline		awShaderDefine();
		ZENInline		awShaderDefine(const char* _zName, const char* _zValue);
		ZENInline		awShaderDefine(const zenString& _zName, const zenString& _zValue);
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
		ZENInline awShaderTexture();
		ZENInline awShaderTexture(zenHash32 _hTextureName, zenResID _TextureID, zenResID _SamplerID);

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
		ZENInline const void* GetData()const {return this + 1;}

	protected:
		ZENInline			awShaderParameter(zenHash32 _hName, zenConst::eShaderElementType _eType, zenU16 _uVectorSize, zenU16 _uArrayCount, zenU16 _uSizeTotal);
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class awShaderParamVector1 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector1, awShaderParameter)
	public:
		ZENInline		awShaderParamVector1( zenHash32 _hName, TType _Value);
		TType		mValue;
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class awShaderParamVector2 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector2, awShaderParameter)
	public:
		ZENInline		awShaderParamVector2( zenHash32 _hName, TType _Value1, TType _Value2); 
		ZENInline		awShaderParamVector2( zenHash32 _hName, const TType _Values[2]);
		ZENInline		awShaderParamVector2( zenHash32 _hName, const zenVector2<TType>& _vValue);
		TType		mValue[2];
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class awShaderParamVector3 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector3, awShaderParameter)
	public:
		ZENInline		awShaderParamVector3( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3); 
		ZENInline		awShaderParamVector3( zenHash32 _hName, const TType _Values[3]);
		ZENInline		awShaderParamVector3( zenHash32 _hName, const zenVector3<TType>& _vValue);
		TType		mValue[3];
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class awShaderParamVector4 : public awShaderParameter
	{
	ZENClassDeclare(awShaderParamVector4, awShaderParameter)
	public:
		ZENInline		awShaderParamVector4( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4);
		ZENInline		awShaderParamVector4( zenHash32 _hName, const TType _Values[4]);
		ZENInline		awShaderParamVector4( zenHash32 _hName, const zenVector4<TType>& _vValues);
		TType		mValue[4];
	};

	typedef awShaderParamVector1<float, zenConst::keShaderElemType_Float> awShaderFloat1;
	typedef awShaderParamVector2<float, zenConst::keShaderElemType_Float> awShaderFloat2;
	typedef awShaderParamVector3<float, zenConst::keShaderElemType_Float> awShaderFloat3;
	typedef awShaderParamVector4<float, zenConst::keShaderElemType_Float> awShaderFloat4;

}} // namespace zen, zenRes

#include "zenResGfxShaderParam.inl"

#endif

