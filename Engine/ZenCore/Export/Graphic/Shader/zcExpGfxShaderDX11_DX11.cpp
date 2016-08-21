#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX11

#include <D3D11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace zcExp
{

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
//! @brief		Process the shader texture infos
//! @details	
//=================================================================================================
ZENInline void ProcessTexture(ID3D11ShaderReflection& _GfxShaderReflection, const D3D11_SHADER_INPUT_BIND_DESC& _InputDesc, ExportGfxShaderDX11::BindInfo& _BindingOut, zHash32& _TextureNameOut)
{
	char zName[128];
	// We package texture access inside a structure that contains bother Texture Buffer and Sampler,
	// so no need to keep track of separate sampler.
	//! @todo optimize might want to revisit 1 sampler per texture
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

ExporterGfxShaderDX11_DX11::ExporterGfxShaderDX11_DX11(const ExportResultRef& _rExportOut)
: ExporterBase(_rExportOut.GetSafe())
, mrExport(_rExportOut)
{
}

//=================================================================================================
//! @brief		Compile the shader code and output the binary
//! @details	
//! @return 	true if successful
//=================================================================================================
bool ExporterGfxShaderDX11_DX11::ExportWorkCompile()
{
	HRESULT		hr(S_OK);
	DWORD		dwShaderFlags(D3DCOMPILE_ENABLE_STRICTNESS);
	ID3DBlob*	pErrorBlob;
	ID3DBlob*	pCompiledBlob;		
	ExportInfoGfxShader* pExportInfo = static_cast<ExportInfoGfxShader*>(mpExportInfo);
	const char* pzShaderStage[zenConst::keShaderStage__Count]={"vs_5_0","ps_5_0"};	//Must match eShaderStage enum
	
	//-------------------------------------------------------------------------
	// Import defines setting for shader compiler preprocessor
	//-------------------------------------------------------------------------
	D3D10_SHADER_MACRO pDefines[128];
	zUInt uDefineCount = pExportInfo->maDefines.Count();
	zenAssertMsg(uDefineCount < ZENArrayCount(pDefines)-2, "Too many defines included, increase capacity");
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

	mrExport->maCompiledShader.Clear();
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
	if( pErrorBlob != nullptr )
	{
		//@todo Missing: Implement logging	strncpy( mzLogDetail, (char*)pErrorBlob->GetBufferPointer(), keLogSizeDetail );
		printf( "\n%s\n", (char*)pErrorBlob->GetBufferPointer() );
		pErrorBlob->Release();
	}
	if( pCompiledBlob )
	{
		if( SUCCEEDED(hr) )
		{
			mrExport->maCompiledShader.Copy( static_cast<zU8*>(pCompiledBlob->GetBufferPointer()), zUInt(pCompiledBlob->GetBufferSize()) );
		}
		pCompiledBlob->Release();
	}

	return mrExport->maCompiledShader.Count() > 0;
}

//=================================================================================================
//! @brief		Extract the texture parameter infos
//! @details	
//! @return 	true if successful
//=================================================================================================
bool ExporterGfxShaderDX11_DX11::ExportWorkExtractResources()
{
	ExportInfoGfxShader*					pExportInfo = static_cast<ExportInfoGfxShader*>(mpExportInfo);
	ExportGfxShaderDX11::BindInfo			aTextureBind[zcExp::kuDX11_TexturePerStageMax];
	zHash32									aTextureName[zcExp::kuDX11_TexturePerStageMax];
	zUInt									uTextureCount(0);
	ID3D11ShaderReflection*					pGfxShaderReflection(nullptr);
	ID3D11ShaderReflectionConstantBuffer*	pConstBuffer(nullptr);
	D3D11_SHADER_DESC						shaderDesc;	
	D3D11_SHADER_INPUT_BIND_DESC			resourceDesc;			
	
	mrExport->meShaderStage			= pExportInfo->mExportResID.meType == zenConst::keResType_GfxShaderPixel ? zenConst::keShaderStage_Pixel : zenConst::keShaderStage_Vertex;
	mrExport->muTextureSlotCount	= 0;
	mrExport->maParamDefID.SetCount(zcExp::keShaderParamFreq__Count);
	if( SUCCEEDED( D3DReflect( mrExport->maCompiledShader.First(), mrExport->maCompiledShader.SizeMem(), IID_ID3D11ShaderReflection, (void**) &pGfxShaderReflection ) ) )
	{
		pGfxShaderReflection->GetDesc( &shaderDesc );
		for( zUInt uResIdx=0; uResIdx<shaderDesc.BoundResources; ++uResIdx )
		{
			if( SUCCEEDED( pGfxShaderReflection->GetResourceBindingDesc(uResIdx, &resourceDesc ) ) )
			{				
				if( resourceDesc.Type ==  D3D_SIT_TEXTURE )
				{
					zenAssert( uTextureCount < kuDX11_TexturePerStageMax );
					ProcessTexture( *pGfxShaderReflection, resourceDesc, aTextureBind[uTextureCount], aTextureName[uTextureCount] );
					mrExport->muTextureSlotCount = zenMath::Max(UINT(mrExport->muTextureSlotCount), resourceDesc.BindPoint+resourceDesc.BindCount);	
					++uTextureCount;
				}
				else if( resourceDesc.Type == D3D_SIT_CBUFFER )
				{
					zenAssert(resourceDesc.BindPoint<zcExp::keShaderParamFreq__Count);
					ProcessShaderParamDef( *pGfxShaderReflection, resourceDesc, mrExport->mResID.GetSource(), mrExport->maParamDefID[resourceDesc.BindPoint] );
				}
			}			
		}
		pGfxShaderReflection->Release();

		mrExport->maTextureSamplerName.Copy( aTextureName, uTextureCount );
		mrExport->maTextureSamplerSlot.Copy( aTextureBind, uTextureCount );		
		return TRUE;
	}

	return FALSE;
}

bool ExporterGfxShaderDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	zenAssert(mrExport.IsValid());
	return ExportWorkCompile() && ExportWorkExtractResources();
}

//=================================================================================================
//! @brief		Make sure all BufferInfo exist
//! @details	Check for the existence of ShaderParam, and if not present, request their creation
//! @return 	true
//=================================================================================================
bool ExporterGfxShaderDX11_DX11::ExportEnd()
{
	if( mpExportInfo->IsSuccess() && Super::ExportEnd() )	
	{
		for(zUInt idx=0; idx<mrExport->maParamDefID.Count(); ++idx)
		{
			if( mrExport->maParamDefID[idx].IsValid() )
				mrExport->maParamDefID[idx] = zcExp::CreateGfxShaderParamDef(mrExport->mResID, static_cast<eShaderParamFreq>(idx));
		}

		if( mrExport->meShaderStage == zenConst::keShaderStage_Vertex )
			mrExport->mShaderInputSignatureID = zcExp::CreateGfxInputSignature(mrExport->mResID);

		return true;
	}	
	return false;
}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX11
