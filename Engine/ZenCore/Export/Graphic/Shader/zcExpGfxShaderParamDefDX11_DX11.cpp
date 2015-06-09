#include "zcCore.h"

#include <D3D11.h>
#include <D3D11Shader.h>
#include <D3Dcompiler.h>

namespace zcExp
{

ExporterGfxShaderParamDefDX11_DX11::ExporterGfxShaderParamDefDX11_DX11(const ResDataRef& _rResData)
: Super(_rResData.GetSafe())
, mrResData(_rResData)
{
}

//=================================================================================================
//! @brief		Load the shader info needed for export
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return		Result code ( >0 if sucessful)
//=================================================================================================
bool ExporterGfxShaderParamDefDX11_DX11::ExportStart()
{	
	if( !Super::ExportStart() )
		return false;

	ExportInfoGfxShaderParamDef*			pExportInfo = static_cast<ExportInfoGfxShaderParamDef*>(mpExportInfo);	
	zEngineConstRef<ResDataGfxShaderDX11>	rShaderItem	= zcDepot::ResourceData.GetItem<ResDataGfxShaderDX11>(pExportInfo->mParentShaderID);
	if( rShaderItem.IsValid() )
	{
		maCompiledShader = rShaderItem->maCompiledShader;
		return true;
	}
	return false;
}



//=================================================================================================
//! @brief		Create a Constant Buffer Info
//! @details	
//! @note		Can be called from Thread:Task or Thread:Main, make sure the method is threadsafe
//-------------------------------------------------------------------------------------------------
//! @param		_bIsTHRTask - True if called from TaskManager Thread:Task
//! @return		Result code ( >0 if sucessful)
//=================================================================================================
bool ExporterGfxShaderParamDefDX11_DX11::ExportWork(bool _bIsTHRTask)
{
	bool bValid(false);
	ID3D11ShaderReflection*					pGfxShaderReflection(NULL);
	ID3D11ShaderReflectionConstantBuffer*	pConstBuffer(NULL);
	D3D11_SHADER_DESC						shaderDesc;
	D3D11_SHADER_BUFFER_DESC				bufferDesc;
	D3D11_SHADER_INPUT_BIND_DESC			ResourceDesc;	
	ExportInfoGfxShaderParamDef*			pExportInfo		= static_cast<ExportInfoGfxShaderParamDef*>(mpExportInfo);
	if( SUCCEEDED( D3DReflect( maCompiledShader.First(), maCompiledShader.Size(), IID_ID3D11ShaderReflection, (void**) &pGfxShaderReflection ) ) )
	{
		// Find the right Constant Buffer
		pGfxShaderReflection->GetDesc( &shaderDesc );
		for( zUInt uResIdx=0; !pConstBuffer && uResIdx<shaderDesc.BoundResources; ++uResIdx )
		{
			HRESULT hr = pGfxShaderReflection->GetResourceBindingDesc(	uResIdx, &ResourceDesc );
			if( SUCCEEDED(hr) && ResourceDesc.Type ==  D3D_SIT_CBUFFER &&  pExportInfo->meBufferIndex == ResourceDesc.BindPoint )
				pConstBuffer = pGfxShaderReflection->GetConstantBufferByName( ResourceDesc.Name );
		}

		// Extract the Constant Buffer Infos
		if( pConstBuffer ) 
		{
			bValid = true;
			pConstBuffer->GetDesc( &bufferDesc );
			mrResData->meFrequence = pExportInfo->meBufferIndex;
			
			// Load the description of each variable for use later on when binding a buffer
			mrResData->mdParameters.Init(32);
			mrResData->maParameterDefaults.SetCount( bufferDesc.Size );
			for( UINT j = 0; bValid && j < bufferDesc.Variables; j++ )
			{
				// Get the variable description and store it
				ID3D11ShaderReflectionVariable* pVariable = pConstBuffer->GetVariableByIndex( j );
				D3D11_SHADER_TYPE_DESC			TypeDesc;	
				D3D11_SHADER_VARIABLE_DESC		VarDesc;
				pVariable->GetDesc( &VarDesc );

				// Get the variable type description and store it
				ID3D11ShaderReflectionType* pType = pVariable->GetType();
				pType->GetDesc( &TypeDesc );

				// Populate our parameter info structure
				zcExp::ShaderParamItemInfo Param;
				switch( TypeDesc.Type )
				{
				case D3D_SVT_FLOAT:	Param.meType = zenConst::keShaderElemType_Float;	break;			
				case D3D_SVT_INT:	Param.meType = zenConst::keShaderElemType_SInt;	break;
				case D3D_SVT_UINT:	Param.meType = zenConst::keShaderElemType_UInt;	break;
				case D3D_SVT_UINT8:	Param.meType = zenConst::keShaderElemType_UByte;	break;
				default: 			bValid = false;									break; 	//! @todo Missing: Support structure
				//default: 			bValid = false; strncpy( mzLogDetail, "Unsupported Shader variable type.", keLogSizeDetail );	break;
				}
				Param.muColCount			= TypeDesc.Columns;
				Param.muRowCount			= TypeDesc.Rows;
				Param.muArrayCount			= TypeDesc.Elements;
				Param.muArrayItemSize		= Param.muArrayCount > 0 ? 4*4 : 0;	//Constant until we add support for structure;
				Param.muOffset				= VarDesc.StartOffset;
				Param.muSize				= VarDesc.Size;
				Param.mbInUse				= (VarDesc.uFlags & D3D_SVF_USED) != 0;

				// Copy default value
				if(VarDesc.DefaultValue)	zenMem::Copy(&mrResData->maParameterDefaults[VarDesc.StartOffset], VarDesc.DefaultValue, VarDesc.Size); 
				else						zenMem::Set(&mrResData->maParameterDefaults[VarDesc.StartOffset], 0, VarDesc.Size); 
				mrResData->mdParameters.GetAdd( zHash32(VarDesc.Name) ) = Param;
			}
			
		}
		else
		{
			//TODO		strncpy( mzLogDetail, "Trying to access an invalid ConstantBuffer index.", keLogSizeDetail );
		}
		pGfxShaderReflection->Release();
	}

	//! @todo Missing : support logging
	//if( !bValid )
	//	strncpy( mzLogDetail, "Failed initializing ConstantBuffer ResourceData", keLogSizeDetail );
	return bValid;
}

}
