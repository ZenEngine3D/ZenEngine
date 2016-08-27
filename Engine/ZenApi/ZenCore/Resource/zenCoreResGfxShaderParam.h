#pragma once
#ifndef __zenApi_Core_ResGfxShaderParam_h__
#define __zenApi_Core_ResGfxShaderParam_h__

namespace zen { namespace zenRes {	

	//=============================================================================================
	//! @class	Used for shader compiling preprocessor
	//=============================================================================================
	class zShaderDefine
	{
	zenClassDeclareNoParent(zShaderDefine)
	public:
		ZENInline	zShaderDefine();
		ZENInline	zShaderDefine(const char* _zName, const char* _zValue);
		ZENInline	zShaderDefine(const zString& _zName, const zString& _zValue);
		zString		mzName;
		zString		mzValue;
	};

	//=============================================================================================
	//! @class	Used when setting the properties of a texture/sampler pair
	//! @todo optim remove sampler/texture automatic depency
	//=============================================================================================
	class zShaderTexture
	{
	zenClassDeclareNoParent(zShaderTexture)
	public:
		ZENInline	zShaderTexture();
		ZENInline	zShaderTexture(zHash32 _hTextureName, zResID _TextureID, zResID _SamplerID);

		zHash32		mhTextureName;
		zResID		mTextureID;
		zResID		mSamplerID;
	};

	//=============================================================================================
	//! @class	Base Class used to configure shader parameters. Useful when creating an array
	//!			of various parameters types that can set at once. Child class configure the content
	//!			of this parent
	//=============================================================================================
	class zShaderParameter
	{
	zenClassDeclareNoParent(zShaderParameter)
	public:
		zHash32		mhName;						//!< Parameter name
		zU32		meType			: 4;		//!< eShaderElementType
		zU32		muVectorSize	: 4;		//!< Number of elements in vector
		zU32		muArrayCount	: 10;		//!< Number of elements in array
		zU32		muSizeTotal		: 16;		//!< Total size of the child class
		ZENInline const void* GetData()const {return this + 1;}

	protected:
		ZENInline			zShaderParameter(zHash32 _hName, zenConst::eShaderElementType _eType, zU16 _uVectorSize, zU16 _uArrayCount, zU16 _uSizeTotal);
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class zShaderParamVector1 : public zShaderParameter
	{
	zenClassDeclare(zShaderParamVector1, zShaderParameter)
	public:
		ZENInline		zShaderParamVector1( zHash32 _hName, TType _Value);
		TType		mValue;
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class zShaderParamVector2 : public zShaderParameter
	{
	zenClassDeclare(zShaderParamVector2, zShaderParameter)
	public:
		ZENInline		zShaderParamVector2( zHash32 _hName, TType _Value1, TType _Value2); 
		ZENInline		zShaderParamVector2( zHash32 _hName, const TType _Values[2]);
		ZENInline		zShaderParamVector2( zHash32 _hName, const zVector2<TType>& _vValue);
		TType		mValue[2];
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class zShaderParamVector3 : public zShaderParameter
	{
	zenClassDeclare(zShaderParamVector3, zShaderParameter)
	public:
		ZENInline		zShaderParamVector3( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3); 
		ZENInline		zShaderParamVector3( zHash32 _hName, const TType _Values[3]);
		ZENInline		zShaderParamVector3( zHash32 _hName, const zVector3<TType>& _vValue);
		TType		mValue[3];
	};

	template <class TType, zenConst::eShaderElementType TShaderType>
	class zShaderParamVector4 : public zShaderParameter
	{
	zenClassDeclare(zShaderParamVector4, zShaderParameter)
	public:
		ZENInline		zShaderParamVector4( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4);
		ZENInline		zShaderParamVector4( zHash32 _hName, const TType _Values[4]);
		ZENInline		zShaderParamVector4( zHash32 _hName, const zVector4<TType>& _vValues);
		TType		mValue[4];
	};

	typedef zShaderParamVector1<float, zenConst::keShaderElemType_Float> zShaderFloat1;
	typedef zShaderParamVector2<float, zenConst::keShaderElemType_Float> zShaderFloat2;
	typedef zShaderParamVector3<float, zenConst::keShaderElemType_Float> zShaderFloat3;
	typedef zShaderParamVector4<float, zenConst::keShaderElemType_Float> zShaderFloat4;

}} // namespace zen, zenRes

#include "zenCoreResGfxShaderParam.inl"

#endif

