#include "zcCore.h"
#if ZEN_EXPORT_OR_RESOURCE_DX12

#include <D3D11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace zcExp
{

//=================================================================================================
//! @brief		Process the shader constant buffer infos
//! @details	
//=================================================================================================
void ProcessCBufferDef(		zArrayStatic<zHash32>&					Out_aCBufferParamName,
							zArrayStatic<GfxCBufferParamInfo>&		Out_aCBufferParamInfo,
							zArrayStatic<zU8>&						Out_aCBufferDefaultValues,
							ID3D11ShaderReflection&					_GfxShaderReflection, 
							ID3D11ShaderReflectionConstantBuffer*	_pBufferReflection,  
							const D3D11_SHADER_BUFFER_DESC&			_BufferDesc )
{		
	Out_aCBufferParamName.SetCount( _BufferDesc.Variables );
	Out_aCBufferParamInfo.SetCount( _BufferDesc.Variables );
	Out_aCBufferDefaultValues.SetCount( _BufferDesc.Size );

	// Find hash of parameters informations
	zResID::NameHash			hName;
	D3D11_SHADER_VARIABLE_DESC	VarDesc;
	D3D11_SHADER_TYPE_DESC		TypeDesc;	
	for ( UINT uParamIdx = 0; uParamIdx < _BufferDesc.Variables; uParamIdx++ )
	{
		// Retrieve variable description and type
		ID3D11ShaderReflectionVariable* pVariable	= _pBufferReflection->GetVariableByIndex( uParamIdx );
		ID3D11ShaderReflectionType* pType			= pVariable->GetType();
		pVariable->GetDesc( &VarDesc );		
		pType->GetDesc( &TypeDesc );

		// Populate our parameter info structure
		bool bCanVector								= false;
		GfxCBufferParamInfo& ParamInfo				= Out_aCBufferParamInfo[uParamIdx];
		Out_aCBufferParamName[uParamIdx]			= zHash32(VarDesc.Name);
		switch( TypeDesc.Type )
		{
		case D3D_SVT_FLOAT:	ParamInfo.meType = keShaderCBuffType_Float;		bCanVector = true; break;			
		case D3D_SVT_INT:	ParamInfo.meType = keShaderCBuffType_Int;		bCanVector = true; break;
		case D3D_SVT_UINT:	ParamInfo.meType = keShaderCBuffType_UInt;		bCanVector = true; break;
		case D3D_SVT_UINT8:	ParamInfo.meType = keShaderCBuffType_Byte;		bCanVector = true; break;
		case D3D_SVT_BOOL:	ParamInfo.meType = keShaderCBuffType_Bool;		bCanVector = true; break;
		default: 			ParamInfo.meType = keShaderCBuffType__Invalid;	bCanVector = true; break; 	//! @todo Missing: Support structure
		}
				
		ParamInfo.meType				+= bCanVector ? (TypeDesc.Columns - 1) : 0;
		//ParamInfo.muColCount			= TypeDesc.Columns;
		ParamInfo.muRowCount			= TypeDesc.Rows;
		ParamInfo.muArrayCount			= TypeDesc.Elements;
		ParamInfo.muArrayItemSize		= ParamInfo.muArrayCount > 0 ? 4*4 : 0;	//Constant until we add support for structure;
		ParamInfo.muOffset				= VarDesc.StartOffset;
		ParamInfo.muSize				= VarDesc.Size;
		ParamInfo.mbInUse				= (VarDesc.uFlags & D3D_SVF_USED) != 0;

		// Copy default value
		if(VarDesc.DefaultValue)	zenMem::Copy(&Out_aCBufferDefaultValues[VarDesc.StartOffset], static_cast<const zU8*>(VarDesc.DefaultValue), VarDesc.Size); 
		else						zenMem::Set(&Out_aCBufferDefaultValues[VarDesc.StartOffset], 0, VarDesc.Size); 
	}
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
	const char* pzShaderStage[]={"vs_5_0","ps_5_0"};	//Must match eShaderStage enum
	const char* pzShaderDefine[keShaderStage__Count]={"SHADER_VERTEX","SHADER_PIXEL"};
	//-------------------------------------------------------------------------
	// Import defines setting for shader compiler preprocessor
	//-------------------------------------------------------------------------
	D3D10_SHADER_MACRO pDefines[128];
	zUInt uUserDefineCount			= pExportInfo->maDefines.Count();
	const zUInt uSystemDefineCount	= 3;
	zenAssertMsg(uUserDefineCount+uSystemDefineCount <= zenArrayCount(pDefines), "Too many defines included, increase capacity");
	zenStaticAssert(zenArrayCount(pzShaderStage) == keShaderStage__Count);
	zenStaticAssert(zenArrayCount(pzShaderDefine) == keShaderStage__Count);

	D3D10_SHADER_MACRO*	pDefineCur	= pDefines;
	uUserDefineCount				= zenMath::Min(uUserDefineCount, zUInt(zenArrayCount(pDefines)-3));	
	if( uUserDefineCount )
	{		
		zenRes::zShaderDefine*	pEntry		= pExportInfo->maDefines.First();
		zenRes::zShaderDefine*	pEntryEnd	= pEntry + uUserDefineCount;
		while( pEntry < pEntryEnd )
		{
			pDefineCur->Name		= pEntry->mzName;
			pDefineCur->Definition	= pEntry->mzValue;
			++pDefineCur;
			++pEntry;
		}
	}
	pDefineCur->Name = "SHADER_DX11";								pDefineCur->Definition	= "1";		pDefineCur++;
	pDefineCur->Name = pzShaderDefine[pExportInfo->meShaderStage];	pDefineCur->Definition	= "1";		pDefineCur++;
	pDefineCur->Name = 0;											pDefineCur->Definition	= nullptr;	pDefineCur++;
	zenAssertMsg(pDefineCur-pDefines == uUserDefineCount+uSystemDefineCount, "System define count doesn't match what was expected");

	//-------------------------------------------------------------------------
	wchar_t zFilename[256]; size_t convertSize;
	mbstowcs_s(&convertSize, zFilename, zenArrayCount(zFilename), pExportInfo->mzFilename, zenArrayCount(zFilename)-1);

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
	ID3D11ShaderReflection*					pGfxShaderReflection(nullptr);	
	
	zenMem::Zero(mrExport->maResourceBindCount);
	zenMem::Zero(mrExport->maResourceBindMax);
	zU32 uCBufferIdx(0);
	mrExport->meShaderStage	= pExportInfo->mExportResID.meType == zenConst::keResType_GfxShaderPixel ? zenConst::keShaderStage_Pixel : zenConst::keShaderStage_Vertex;

	if( SUCCEEDED( D3DReflect( mrExport->maCompiledShader.First(), mrExport->maCompiledShader.SizeMem(), IID_ID3D11ShaderReflection, (void**) &pGfxShaderReflection ) ) )
	{
		D3D11_SHADER_DESC shaderDesc;
		zUInt uBoundCBufferCount(0);
		pGfxShaderReflection->GetDesc( &shaderDesc );
		mrExport->maResourceBinding.SetCount( shaderDesc.BoundResources );

		// Can't rely on 'shaderDesc.ConstantBuffers' for CBuffer count, since list even unbound one
		// So first find out how many there is
		for( UINT uResIdx=0; uResIdx<shaderDesc.BoundResources; ++uResIdx )
		{
			D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
			if( SUCCEEDED( pGfxShaderReflection->GetResourceBindingDesc(uResIdx, &resourceDesc ) ) && resourceDesc.Type == D3D_SIT_CBUFFER )
				++uBoundCBufferCount;
		}

		mrExport->maCBufferResIndex.SetCount( uBoundCBufferCount );
		maCBufferName.SetCount( uBoundCBufferCount );
		maCBufferParamName.SetCount( uBoundCBufferCount );
		maCBufferParamInfo.SetCount( uBoundCBufferCount );
		maCBufferDefaultValues.SetCount( uBoundCBufferCount );
		
		for( UINT uResIdx=0; uResIdx<shaderDesc.BoundResources; ++uResIdx )
		{
			D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
			ExportGfxShader::ShaderBindInfo& ResInfo	= mrExport->maResourceBinding[uResIdx];
			ResInfo.meType							= keShaderRes_Unknown;
			ResInfo.muSlotIndex						= 0;
			ResInfo.muSlotCount						= 0;
			ResInfo.muMemSize						= 0;

			if( SUCCEEDED( pGfxShaderReflection->GetResourceBindingDesc(uResIdx, &resourceDesc ) ) )
			{								
				ResInfo.mzName			= resourceDesc.Name;
				ResInfo.muSlotIndex		= resourceDesc.BindPoint;
				ResInfo.muSlotCount		= resourceDesc.BindCount;
				if ( resourceDesc.Type ==  D3D_SIT_TEXTURE )
				{
					ResInfo.meType = keShaderRes_Texture;
				}
				else if ( resourceDesc.Type ==  D3D_SIT_SAMPLER )
				{
					ResInfo.meType = keShaderRes_Sampler;
				}
				else if( resourceDesc.Type == D3D_SIT_CBUFFER )
				{
					ID3D11ShaderReflectionConstantBuffer* pBufferReflect = pGfxShaderReflection->GetConstantBufferByName( resourceDesc.Name );
					D3D11_SHADER_BUFFER_DESC bufferDesc;
					pBufferReflect->GetDesc( &bufferDesc );
					//zenAssert(resourceDesc.BindPoint<zcExp::keShaderParamFreq__Count);					
					zenAssertMsg( resourceDesc.BindCount == 1, "No current support for array of constants buffer");					
					ProcessCBufferDef( maCBufferParamName[uCBufferIdx], maCBufferParamInfo[uCBufferIdx], maCBufferDefaultValues[uCBufferIdx], *pGfxShaderReflection, pBufferReflect, bufferDesc );
					maCBufferName[uCBufferIdx]					= bufferDesc.Name;
					mrExport->maCBufferResIndex[uCBufferIdx++]	= static_cast<zU8>(uResIdx);					
					ResInfo.meType								= keShaderRes_CBuffer;
					ResInfo.muMemSize							= bufferDesc.Size;
				}
				else if( resourceDesc.Type == D3D_SIT_STRUCTURED)
				{
					ID3D11ShaderReflectionConstantBuffer* pBufferReflect = pGfxShaderReflection->GetConstantBufferByName( resourceDesc.Name );
					D3D11_SHADER_BUFFER_DESC bufferDesc;
					pBufferReflect->GetDesc( &bufferDesc );		
					ResInfo.meType		= keShaderRes_Buffer;
					ResInfo.muMemSize	= bufferDesc.Size;
				}		
			}

			mrExport->maResourceBindCount[ResInfo.meType]	+= 1;
			mrExport->maResourceBindMax[ResInfo.meType]		= zenMath::Max<zU32>(mrExport->maResourceBindMax[ResInfo.meType], ResInfo.muSlotIndex + ResInfo.muSlotCount );
		}		
		pGfxShaderReflection->Release();
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
		const zUInt bufferCount(maCBufferParamName.Count());
		mrExport->maCBufferParentID.SetCount( bufferCount );
		for(zUInt bufferIdx(0); bufferIdx<bufferCount; ++bufferIdx )
			mrExport->maCBufferParentID[bufferIdx] = zcExp::CreateGfxCBufferDefinition(maCBufferName[bufferIdx], maCBufferParamName[bufferIdx], maCBufferParamInfo[bufferIdx], maCBufferDefaultValues[bufferIdx] );

		return true;
	}	
	return false;
}

}

#endif //ZEN_EXPORT_OR_RESOURCE_DX12
