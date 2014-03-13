#include "libZenCore.h"
#if AW_EXPORT_OR_RESOURCE_DX11

#include <D3D11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace EExp
{

//=================================================================================================
//! @brief		Process the shader texture infos
//! @details	
//=================================================================================================
ZENInline void ProcessTexture(ID3D11ShaderReflection& _GfxShaderReflection, const D3D11_SHADER_INPUT_BIND_DESC& _InputDesc, SerialShader_DX11::BindInfo& _BindingOut, zHash32& _TextureNameOut)
{
	char zName[128];
	// We package texture access inside a structure that contains bother Texture Buffer and Sampler,
	// so no need to keep track of separate sampler.
	LPCSTR pNameSrc(_InputDesc.Name);
	char* pNameDst(zName);
	while( *pNameSrc != 0 && *pNameSrc != '.' )
		*pNameDst++ = *pNameSrc++;
	*pNameDst = 0;

	// Retrieve texture/sampler infos (name, slot, ...)	
	_BindingOut.uSlot	= _InputDesc.BindPoint;
	_BindingOut.uCount	= _InputDesc.BindCount;
	_TextureNameOut		= zHash32(zName);
}

//=================================================================================================
//! @brief		Process the shader constant buffer infos
//! @details	
//=================================================================================================
ZENInline void ProcessShaderParamDef(ID3D11ShaderReflection& _GfxShaderReflection, const D3D11_SHADER_INPUT_BIND_DESC& _InputDesc, zenConst::eResSource _eSource, zResID& _ShaderParaDefIDOut)
{
	D3D11_SHADER_BUFFER_DESC				bufferDesc;
	D3D11_SHADER_VARIABLE_DESC				VarDesc;
	ID3D11ShaderReflectionConstantBuffer*	pBufferReflection = _GfxShaderReflection.GetConstantBufferByName( _InputDesc.Name );
	pBufferReflection->GetDesc( &bufferDesc );

	zResID::NameHash hName(bufferDesc.Name);
	for ( UINT uParamIdx = 0; uParamIdx < bufferDesc.Variables; uParamIdx++ )
	{
		ID3D11ShaderReflectionVariable* pVariable = pBufferReflection->GetVariableByIndex( uParamIdx );			
		pVariable->GetDesc( &VarDesc );
		hName.Append(VarDesc.Name);
		hName.Append(&VarDesc.Size, sizeof(VarDesc.Size));
		hName.Append(&VarDesc.StartOffset, sizeof(VarDesc.StartOffset));
	}

	_ShaderParaDefIDOut = zResID(hName, zenConst::keResPlatform_DX11, zenConst::keResType_GfxShaderParamDef, _eSource );
}

//=================================================================================================
//! @brief		Compile the shader code and output the binary
//! @details	
//! @return 	true if successful
//=================================================================================================
bool SerialShader_DX11::ExportWorkCompile()
{
	HRESULT		hr(S_OK);
	DWORD		dwShaderFlags(D3DCOMPILE_ENABLE_STRICTNESS);
	ID3DBlob*	pErrorBlob;
	ID3DBlob*	pCompiledBlob;		
	ExportInfo* pExportInfo = static_cast<ExportInfo*>(mpExportInfo);
	const char* pzShaderStage[EExp::keShaderStage__Count]={"vs_5_0","ps_5_0"};	//Must match eShaderStage enum
	
	//-------------------------------------------------------------------------
	// Import defines setting for shader compiler preprocessor
	//-------------------------------------------------------------------------
	D3D10_SHADER_MACRO pDefines[128];
	zUInt uDefineCount = pExportInfo->maDefines.Count();
	ZENAssertMsg(uDefineCount < ZENArrayCount(pDefines)-2, "Too many defines included, increase capacity");
	uDefineCount		= zenMath::Min(uDefineCount, zUInt(ZENArrayCount(pDefines)-2));
	if( uDefineCount )
	{
		D3D10_SHADER_MACRO*		pDefineCur	= pDefines;
		zenRes::zShaderDefine*	pEntry		= pExportInfo->maDefines.First();
		zenRes::zShaderDefine*	pEntryEnd	= pEntry + uDefineCount;
		while( pEntry < pEntryEnd )
		{
			pDefineCur->Name		= pEntry->mzName;
			pDefineCur->Definition	= pEntry->mzValue;
			++pDefineCur;
			++pEntry;
		}
	}
	pDefines[uDefineCount].Name			= "SHADER_DX11";
	pDefines[uDefineCount].Definition	= "1";
	pDefines[uDefineCount+1].Name		= 0;
	pDefines[uDefineCount+1].Definition	= 0;

	//-------------------------------------------------------------------------
	wchar_t zFilename[256];
	mbstowcs(zFilename, pExportInfo->mzFilename, sizeof(zFilename));

	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shader.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shader to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	
	mSerialCommon.maCompiledShader.Clear();
	hr = D3DCompileFromFile( 
		zFilename, 
		pDefines, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		pExportInfo->mzEntryname, 
		pzShaderStage[pExportInfo->meShaderStage], 
		dwShaderFlags, 
		0, 
		&pCompiledBlob, 
		&pErrorBlob );	
	if( pErrorBlob != NULL )
	{
		//@todo Implement logging	strncpy( mzLogDetail, (char*)pErrorBlob->GetBufferPointer(), keLogSizeDetail );
		printf( "\n%s\n", (char*)pErrorBlob->GetBufferPointer() );
		pErrorBlob->Release();
	}
	if( pCompiledBlob )
	{
		if( SUCCEEDED(hr) )
		{
			mSerialCommon.maCompiledShader.Copy( static_cast<zU8*>(pCompiledBlob->GetBufferPointer()), zUInt(pCompiledBlob->GetBufferSize()) );
		}
		pCompiledBlob->Release();
	}
	
	return mSerialCommon.maCompiledShader.Count() > 0;
}

//=================================================================================================
//! @brief		Extract the texture parameter infos
//! @details	
//! @return 	true if successful
//=================================================================================================
bool SerialShader_DX11::ExportWorkExtractResources()
{
	ExportInfo*								pExportInfo = static_cast<ExportInfo*>(mpExportInfo);
	BindInfo								aTextureBind[EExp::kuDX11_TexturePerStageMax];
	zHash32								aTextureName[EExp::kuDX11_TexturePerStageMax];
	zUInt									uTextureCount(0);
	ID3D11ShaderReflection*					pGfxShaderReflection(NULL);
	ID3D11ShaderReflectionConstantBuffer*	pConstBuffer(NULL);
	D3D11_SHADER_DESC						shaderDesc;	
	D3D11_SHADER_INPUT_BIND_DESC			resourceDesc;			
	
	muTextureSlotCount = 0;
	maParamDefID.SetCount(EExp::keShaderParamFreq__Count);
	if( SUCCEEDED( D3DReflect( mSerialCommon.maCompiledShader.First(), mSerialCommon.maCompiledShader.Size(), IID_ID3D11ShaderReflection, (void**) &pGfxShaderReflection ) ) )
	{
		pGfxShaderReflection->GetDesc( &shaderDesc );
		for( zUInt uResIdx=0; uResIdx<shaderDesc.BoundResources; ++uResIdx )
		{
			if( SUCCEEDED( pGfxShaderReflection->GetResourceBindingDesc(uResIdx, &resourceDesc ) ) )
			{				
				if( resourceDesc.Type ==  D3D_SIT_TEXTURE )
				{
					ZENAssert( uTextureCount < EExp::kuDX11_TexturePerStageMax );
					ProcessTexture( *pGfxShaderReflection, resourceDesc, aTextureBind[uTextureCount], aTextureName[uTextureCount] );
					muTextureSlotCount = zenMath::Max(UINT(muTextureSlotCount), resourceDesc.BindPoint+resourceDesc.BindCount);	
					++uTextureCount;
				}
				else if( resourceDesc.Type == D3D_SIT_CBUFFER )
				{
					ZENAssert(resourceDesc.BindPoint<EExp::keShaderParamFreq__Count);
					ProcessShaderParamDef( *pGfxShaderReflection, resourceDesc, mResID.Source(), maParamDefID[resourceDesc.BindPoint] );
				}
			}			
		}
		pGfxShaderReflection->Release();

		maTextureSamplerName.Copy( aTextureName, uTextureCount );
		maTextureSamplerSlot.Copy( aTextureBind, uTextureCount );		
		return TRUE;
	}

	return FALSE;
}

//=================================================================================================
//! @brief		Make sure all BufferInfo exist
//! @details	Check for the existence of ShaderParam, and if not present, request their creation
//! @return 	true
//=================================================================================================
bool SerialShader_DX11::ExportEnd()
{
	if( mpExportInfo->IsSuccess() && Super::ExportEnd() )
	{
		for(zUInt idx=0; idx<maParamDefID.Count(); ++idx)
		{
			if( maParamDefID[idx].IsValid() )
				maParamDefID[idx] = EExp::CreateGfxShaderParamDef(mResID, static_cast<eShaderParamFreq>(idx));
		}
		return true;
	}	
	return false;
}

bool SerialGfxShaderPixel_DX11::ExportWork(bool _bIsTHRTask)
{
	meShaderStage = keShaderStage_Pixel;
	return ExportWorkCompile() && ExportWorkExtractResources();
}


bool SerialGfxShaderVertex_DX11::ExportWork(bool _bIsTHRTask)
{
	meShaderStage = keShaderStage_Vertex;
	return ExportWorkCompile() && ExportWorkExtractResources();
}

//=================================================================================================
//! @brief		Make sure all Signature Export exist too
//! @details	Check for the existence of Signature Assert/export, and if not present,  
//!				request its creation
//! @return 	true
//=================================================================================================
bool SerialGfxShaderVertex_DX11::ExportEnd()
{
	if( mpExportInfo->IsSuccess() && Super::ExportEnd() )
	{		
		mSerial.mShaderInputSignatureID	= EExp::CreateGfxInputSignature( mResID );
		return TRUE;
	}				
	
	return FALSE;
}

bool SerialGfxShaderVertex_DX11::Serialize( EExp::Serializer_Base& _Serializer )
{
	return FALSE; 
}

bool SerialGfxShaderPixel_DX11::Serialize( EExp::Serializer_Base& _Serializer )
{
	return FALSE; 
}

}

#endif //AW_EXPORT_OR_RESOURCE_DX11
