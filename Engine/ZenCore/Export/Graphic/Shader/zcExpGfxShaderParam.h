#pragma once

namespace zcExp
{
//==================================================================================================
// Expected parameters for CBuffer creation
//==================================================================================================
struct ExportInfoGfxShaderParam : public ExportInfoBase
{
	zResID	mParentParamDefID;
};

//==================================================================================================
// Base data of a CBUffer send to resource creation
//==================================================================================================
class ExportGfxCBuffer : public zenRes::zExportData
{
zenClassDeclare(ExportGfxCBuffer, zenRes::zExportData)
public:
	zResID				mParentParamDefID;	//!< Parent ShaderParam definition
	zArrayDyn<zU8>		maParameterValues;	//!< Bytes data for all buffer values (see ShaderParamDef for each parameter start offset in array)
};

//==================================================================================================
// Create a new CBuffer 
//==================================================================================================
class ExporterGfxCBuffer : public ExporterBase
{
zenClassDeclare(ExporterGfxCBuffer, ExporterBase)
public:
typedef zEngineRef<ExportGfxCBuffer>	ExportResultRef; //todo use proper ref class
										ExporterGfxCBuffer(const ExportResultRef& _rExportOut);
protected:	
	virtual bool						ExportStart();
	ExportResultRef						mrExport;
};

//==================================================================================================
// CBuffer creation
//==================================================================================================
zResID	CreateGfxCBuffer(zResID _ParentParamDefID);							//!< @brief Create a new Constant Buffer
zResID	CreateGfxCBuffer(zResID _ParentShaderID, zU8 _uCBufferSlot);		//!< @brief Create a new Constant Buffer
zResID	CreateGfxCBuffer(zResID _ParentShaderID, zHash32 _hCBufferName);	//!< @brief Create a new Constant Buffer

//==================================================================================================
// Various Pre-defined shader parameter type that can be used to assign value to CBuffers.
// They allow settting multiple values of different type at the same time
//==================================================================================================
class ParameterBase
{
zenClassDeclareNoParent(ParameterBase)
public:
 	zHash32		mhName;						//!< Parameter name
	zU32		meType			: 4;		//!< eShaderElementType
	zU32		muVectorSize	: 4;		//!< Number of elements in vector
	zU32		muArrayCount	: 10;		//!< Number of elements in array
	zU32		muSizeTotal		: 16;		//!< Total size of the child class
	const void*	GetData()const {return this + 1;}

protected:
				ParameterBase(zHash32 _hName, zenConst::eShaderElementType _eType, zU16 _uVectorSize, zU16 _uArrayCount, zU16 _uSizeTotal)
				: mhName(_hName)
				, meType( static_cast<zU16>(_eType))
				, muVectorSize(_uVectorSize)
				, muArrayCount(_uArrayCount)
				, muSizeTotal(_uSizeTotal)
				{}	
};

template <class TType, zenConst::eShaderElementType TShaderType>
class ParameterVector1 : public ParameterBase
{
zenClassDeclare(ParameterVector1, ParameterBase)
public:
				ParameterVector1( zHash32 _hName, TType _Value) 
				: ParameterBase(_hName, TShaderType, 1, 0, sizeof(mValue))
				, mValue(_Value){}	
	TType		mValue;									
};

template <class TType, zenConst::eShaderElementType TShaderType>
class ParameterVector2 : public ParameterBase
{
zenClassDeclare(ParameterVector2, ParameterBase)
public:
				ParameterVector2( zHash32 _hName, TType _Value1, TType _Value2) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
				}	
				ParameterVector2( zHash32 _hName, TType _Values[2]) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
				}
				ParameterVector2( zHash32 _hName, zVector2<TType> _vValue) 
				: ParameterBase(_hName, TShaderType, 2, 0, sizeof(mValue))
				{
					mValue[0] = _vValue.xy[0];
					mValue[1] = _vValue.xy[1];
				}
	TType		mValue[2];
};

template <class TType, zenConst::eShaderElementType TShaderType>
class ParameterVector3 : public ParameterBase
{
zenClassDeclare(ParameterVector3, ParameterBase)
public:
				ParameterVector3( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
					mValue[2] = _Value3;
				}
				ParameterVector3( zHash32 _hName, TType _Values[3]) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
					mValue[2] = _Values[2];
				}
				ParameterVector3( zHash32 _hName, zVector3<TType> _vValue) 
				: ParameterBase(_hName, TShaderType, 3, 0, sizeof(mValue))
				{
					mValue[0] = _vValue.xyz[0];
					mValue[1] = _vValue.xyz[1];
					mValue[2] = _vValue.xyz[2];
				}
	TType		mValue[3];
};

template <class TType, zenConst::eShaderElementType TShaderType>
class ParameterVector4 : public ParameterBase
{
zenClassDeclare(ParameterVector4, ParameterBase)
public:
				ParameterVector4( zHash32 _hName, TType _Value1, TType _Value2, TType _Value3, TType _Value4) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _Value1;
					mValue[1] = _Value2;
					mValue[2] = _Value3;
					mValue[3] = _Value4;
				}	
				ParameterVector4( zHash32 _hName, TType _Values[4]) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _Values[0];
					mValue[1] = _Values[1];
					mValue[2] = _Values[2];
					mValue[3] = _Values[3];
				}
				ParameterVector4( zHash32 _hName, zVector4<TType>& _vValues) 
				: ParameterBase(_hName, TShaderType, 4, 0, sizeof(mValue))
				{
					mValue[0] = _vValues.xyzw[0];
					mValue[1] = _vValues.xyzw[1];
					mValue[2] = _vValues.xyzw[2];
					mValue[3] = _vValues.xyzw[3];
				}
	TType		mValue[4];
};
typedef ParameterVector1< float, zenConst::keShaderElemType_Float > ParameterFloat1;
typedef ParameterVector2< float, zenConst::keShaderElemType_Float > ParameterFloat2;
typedef ParameterVector3< float, zenConst::keShaderElemType_Float > ParameterFloat3;
typedef ParameterVector4< float, zenConst::keShaderElemType_Float > ParameterFloat4;


}
