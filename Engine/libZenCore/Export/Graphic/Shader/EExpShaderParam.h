#pragma once
#ifndef __LibExport_Graphics_ShaderParam_h__
#define __LibExport_Graphics_ShaderParam_h__

namespace EExp
{
	
class ParameterBase
{
AWClassDeclareNoParent(ParameterBase)
public:
 	zenHash32		mhName;						//!< Parameter name
	zenU32			meType			: 4;		//!< eShaderElementType
	zenU32			muVectorSize	: 4;		//!< Number of elements in vector
	zenU32			muArrayCount	: 10;		//!< Number of elements in array
	zenU32			muSizeTotal		: 16;		//!< Total size of the child class
	const void*	GetData()const {return this + 1;}

protected:
				ParameterBase(zenHash32 _hName, awconst::eShaderElementType _eType, zenU16 _uVectorSize, zenU16 _uArrayCount, zenU16 _uSizeTotal)
				: mhName(_hName)
				, meType( static_cast<zenU16>(_eType))
				, muVectorSize(_uVectorSize)
				, muArrayCount(_uArrayCount)
				, muSizeTotal(_uSizeTotal)
				{}	
};

template <class TType, awconst::eShaderElementType TShaderType>
class ParameterVector1 : public ParameterBase
{
AWClassDeclare(ParameterVector1, ParameterBase)
public:
				ParameterVector1( zenHash32 _hName, TType _Value) 
				: ParameterBase(_hName, TShaderType, 1, 0, sizeof(mValue))
				, mValue(_Value){}	
	TType		mValue;									
};

template <class TType, awconst::eShaderElementType TShaderType>
class ParameterVector2 : public ParameterBase
{
AWClassDeclare(ParameterVector2, ParameterBase)
public:
				ParameterVector2( zenHash32 _hName, TType _Value1, TType _Value2) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
				}	
				ParameterVector2( zenHash32 _hName, TType _Values[2]) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
				}
				ParameterVector2( zenHash32 _hName, zenVector2<TType> _vValue) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _vValue.xy[0];
					mValue[1] = _vValue.xy[1];
				}
	TType		mValue[2];
};

template <class TType, awconst::eShaderElementType TShaderType>
class ParameterVector3 : public ParameterBase
{
AWClassDeclare(ParameterVector3, ParameterBase)
public:
				ParameterVector3( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
					mValue[2] = _Value3;
				}
				ParameterVector3( zenHash32 _hName, TType _Values[3]) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
					mValue[2] = _Values[2];
				}
				ParameterVector3( zenHash32 _hName, zenVector3<TType> _vValue) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _vValue.xyz[0];
					mValue[1] = _vValue.xyz[1];
					mValue[2] = _vValue.xyz[2];
				}
	TType		mValue[3];
};

template <class TType, awconst::eShaderElementType TShaderType>
class ParameterVector4 : public ParameterBase
{
AWClassDeclare(ParameterVector4, ParameterBase)
public:
				ParameterVector4( zenHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
					mValue[2] = _Value3;
					mValue[3] = _Value4;
				}	
				ParameterVector4( zenHash32 _hName, TType _Values[4]) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
					mValue[2] = _Values[2];
					mValue[3] = _Values[3];
				}
				ParameterVector4( zenHash32 _hName, zenVector4<TType>& _vValues) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _vValues.xyzw[0];
					mValue[1] = _vValues.xyzw[1];
					mValue[2] = _vValues.xyzw[2];
					mValue[3] = _vValues.xyzw[3];
				}
	TType		mValue[4];
};
typedef ParameterVector1< float, awconst::keShaderElemType_Float > ParameterFloat1;
typedef ParameterVector2< float, awconst::keShaderElemType_Float > ParameterFloat2;
typedef ParameterVector3< float, awconst::keShaderElemType_Float > ParameterFloat3;
typedef ParameterVector4< float, awconst::keShaderElemType_Float > ParameterFloat4;


class SerialShaderParam_Base : public EExp::ExportItem
{
AWClassDeclare(SerialShaderParam_Base, EExp::ExportItem)
public:
	struct ExportInfo : public EExp::ExportInfoBase
	{		
		zenArrayStatic<const EExp::ParameterBase*> maParamValues;
		zenResID								mParentParamDefID;
	};
	struct SerialUseOnly
	{
		zenResID mParentParamDefID;	//!< Parent ShaderParam definition
	};

	SerialUseOnly	mSerial;
};

zenResID	CreateGfxShaderParam(zenResID _ParentParamDefID, const zenArrayBase<const EExp::ParameterBase*>& _aParamValues);
zenResID	CreateGfxShaderParam(zenResID _ParentShaderID, EExp::eShaderParamFreq _eShaderParamIndex, const zenArrayBase<const EExp::ParameterBase*>& _aParamValues=zenArrayStatic<const EExp::ParameterBase*>());

}

#include "EExpShaderParamDX11.h"

#endif