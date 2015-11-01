#include "zcCore.h"


#include <D3D11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace zcExp
{

zResID ExporterGfxInputSignatureDX11_DX11::CallbackGetItemID(zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource, const zcExp::ExportInfoBase* _pExportInfo, bool& _bExistOut)
{
	ZENAssert(_ePlatform==zenConst::keResPlatform_DX11 && _eType==zenConst::keResType_GfxInputSignature);
	ZENAssert( _pExportInfo );
	zResID::NameHash hName;
	const ExportInfoGfxInputSignature* pExportInfo		= static_cast<const ExportInfoGfxInputSignature*>(_pExportInfo);
	zEngineConstRef<ResDataGfxShaderDX11> rParentShader = zcDepot::ResourceData.GetItem<ResDataGfxShaderDX11>( pExportInfo->mParentShaderID );
	if( rParentShader.IsValid() )
	{
		ID3D11ShaderReflection* pGfxShaderReflection = NULL;	
		if ( SUCCEEDED( D3DReflect( &rParentShader->maCompiledShader[0], rParentShader->maCompiledShader.SizeMem(), IID_ID3D11ShaderReflection, (void**)&pGfxShaderReflection ) ) ) 
		{
			// Get zResID of Input Signature
			D3D11_SHADER_DESC shaderDesc;
			pGfxShaderReflection->GetDesc( &shaderDesc );
			for ( zU32 i=0; i< shaderDesc.InputParameters; i++ )
			{
				D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
				pGfxShaderReflection->GetInputParameterDesc(i, &paramDesc );
				hName.Append(paramDesc.SemanticName);
				hName.Append((zU8*)&paramDesc.SemanticIndex, sizeof(paramDesc.SemanticIndex) );
				hName.Append((zU8*)&paramDesc.Register, sizeof(paramDesc.Register) );
				hName.Append((zU8*)&paramDesc.SystemValueType, sizeof(paramDesc.SystemValueType) );
				hName.Append((zU8*)&paramDesc.ComponentType, sizeof(paramDesc.ComponentType) );
				hName.Append((zU8*)&paramDesc.Mask, sizeof(paramDesc.Mask) );
				//SignatureID.Append((zU8*)&paramDesc.ReadWriteMask, sizeof(paramDesc.ReadWriteMask) );		
			}		
			pGfxShaderReflection->Release();	 //Free allocation shader reflection memory
		}
	}
	return zcExp::ValidateItemID(_ePlatform, _eType, _eSource, hName, _bExistOut);
}

ExporterGfxInputSignatureDX11_DX11::ExporterGfxInputSignatureDX11_DX11(const ResDataRef& _rResData)
: ExporterBase(_rResData.GetSafe())
, mrResData(_rResData)
{
}

//=================================================================================================
//! @brief		Load the shader binary needed to create this input signature
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		True if successful
//=================================================================================================
bool ExporterGfxInputSignatureDX11_DX11::ExportStart()
{
	if( !Super::ExportStart() )
		return false;

	ExportInfoGfxInputSignature*			pExportInfo		= static_cast<ExportInfoGfxInputSignature*>(mpExportInfo);
	zEngineConstRef<ResDataGfxShaderDX11>	rShaderParent	= zcDepot::ResourceData.GetItem<zcExp::ResDataGfxShaderDX11>(pExportInfo->mParentShaderID);
	if( rShaderParent.IsValid() )
		maParentCompiledShader = rShaderParent->maCompiledShader;
	
	return maParentCompiledShader.Count() > 0;
}

//=================================================================================================
//! @brief		Create a Vertex Input Signature
//! @details	Creates a dummy eppty shader with only the input declared, so any vertex shader
//!				having the same signature, can use the same input layout object, and not keep 
//!				shader code around
//! @note		Can be called from Thread:Main or Thread:Task
//-------------------------------------------------------------------------------------------------
//! @return		True if successful
//=================================================================================================
bool ExporterGfxInputSignatureDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	ExportInfoGfxInputSignature*	pExportInfo	= static_cast<ExportInfoGfxInputSignature*>(mpExportInfo);
	
	//---------------------------------------------------------------------------------------------
	// Generate dummy shader code
	//---------------------------------------------------------------------------------------------
	ID3D11ShaderReflection* pGfxShaderReflection = NULL;	
	bool bSuccess(FALSE);
	if ( SUCCEEDED( D3DReflect( maParentCompiledShader.First(), maParentCompiledShader.SizeMem(), IID_ID3D11ShaderReflection, (void**)&pGfxShaderReflection ) ) ) 
	{	
		char zShaderText[4096];

		//-----------------------------------------------------------------------------------------
		// Create dummy shader code
		//-----------------------------------------------------------------------------------------		
		const char* zVarTypes[4]={"","uint","int","float"};	//Match D3D10_REGISTER_COMPONENT_TYPE
		D3D11_SHADER_DESC shaderDesc;
		pGfxShaderReflection->GetDesc( &shaderDesc );
		strncpy(zShaderText, "struct VS_INPUT {\n", sizeof(zShaderText) );		
		for ( zU32 i=0; i<shaderDesc.InputParameters; ++i )
		{			
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pGfxShaderReflection->GetInputParameterDesc(i, &paramDesc );
			int iVectorSize = zenMath::BitsScanReverse(paramDesc.Mask)+1;
			_snprintf( &zShaderText[strlen(zShaderText)], sizeof(zShaderText)-strlen(zShaderText), "    %s%i input%i : %s%i;\n", zVarTypes[paramDesc.ComponentType], iVectorSize, i, paramDesc.SemanticName, paramDesc.SemanticIndex);			
		}
		strncpy( &zShaderText[strlen(zShaderText)], "};\n\nvoid main( VS_INPUT input, out float4 Pos : SV_POSITION){\n    Pos = float4(0.0,0.0,0.0,0.0); \n}", sizeof(zShaderText)-strlen(zShaderText) );
		pGfxShaderReflection->Release(); //Free allocation shader reflection memory	

		//----------------------------------------------------------------------------
		// Compile dummy shader
		//----------------------------------------------------------------------------
		DWORD		dwShaderFlags(D3DCOMPILE_ENABLE_STRICTNESS);		
		ID3DBlob*	pErrorBlob;		
		ID3DBlob*	pShaderCompiled;
		if( SUCCEEDED(D3DCompile( zShaderText, strlen(zShaderText), NULL, NULL, NULL, "main", "vs_5_0", dwShaderFlags, 0, &pShaderCompiled, &pErrorBlob) ) )
		{
			mrResData->maDummyShaderCode.Copy( (zU8*)pShaderCompiled->GetBufferPointer(), zUInt(pShaderCompiled->GetBufferSize())  );
			bSuccess = TRUE;
		}
		
		if( pShaderCompiled )	
			pShaderCompiled->Release();

		if( pErrorBlob )
		{
			//TODO strncpy( mzLogDetail, (char*)pErrorBlob->GetBufferPointer(), keLogSizeDetail );					
			pErrorBlob->Release();
		}
	}

	return bSuccess;
}
}
