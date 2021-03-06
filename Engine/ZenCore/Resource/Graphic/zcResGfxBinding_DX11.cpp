#include "zcCore.h"

namespace zcRes
{

bool IsSystemCBuffer( zResID _CBufferDefId, zHash32 _hName )
{
	const zHash32 hVertexInput("cbVertexInput");
	return _hName == hVertexInput;
}


//=================================================================================================

bool GfxMesh_DX11::Initialize()
{
	marGfxMeshStrip.resize( maMeshStripID.size() );
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx] = maMeshStripID[stripIdx];

	return true;
}

void GfxMesh_DX11::SetValue(const zArray<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_aValues);
}

void GfxMesh_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetValue(_Value);
}

void GfxMesh_DX11::SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetResource(_hResourceName, _rResource, _uIndex);
}

void GfxMesh_DX11::SetResource(zHash32 _hResourceName, const zArray<GfxShaderResourceRef>& _arResources)
{
	for(zUInt stripIdx(0), stripCount(marGfxMeshStrip.size()); stripIdx<stripCount; ++stripIdx)
		marGfxMeshStrip[stripIdx]->SetResource(_hResourceName, _arResources);
}
//=================================================================================================

bool GfxMeshStrip_DX11::Initialize()
{
	zenMem::Zero(mhShaderResourceStamp);

	mrIndexBuffer			= mIndexBufferID;
	mrShaderBinding			= mShaderBindingID;

 	marConstantBuffer.resize(maConstanBufferID.size());
 	for(zUInt idx(0), count(marConstantBuffer.size()); idx<count; ++idx)
 		marConstantBuffer[idx] = maConstanBufferID[idx];

	//! @todo Missing: assign default resource for the one not assigned
	for(zUInt stageIdx(0); stageIdx<keShaderStage__Count; ++stageIdx )
		for(zUInt resTypeIdx(0); resTypeIdx<keShaderRes__Count; ++resTypeIdx)
			marShaderResources[stageIdx][resTypeIdx].Copy( maResourceID[stageIdx][resTypeIdx] );

	SetValue(zHash32("VtxInput_Offset"), muVertexFirst );
	return true;
}

void GfxMeshStrip_DX11::SetValue(const zArray<const zenRes::zShaderParameter*>& _aValues)
{
	for(zUInt valIdx(0), valCount(_aValues.size()); valIdx<valCount; ++valIdx)
	{
		const zenRes::zShaderParameter* pValue	= _aValues[valIdx];
		auto paramMask							= mrShaderBinding.HAL()->mdCBufferParamMask[pValue->mhName];
		while( paramMask != 0 )
		{
			zUInt cbufferIdx = zenMath::BitsScan( paramMask );
			paramMask		^= (1<<cbufferIdx);
			marConstantBuffer[cbufferIdx]->SetValue(*pValue);
		}
	}
}

void GfxMeshStrip_DX11::SetValue(const zenRes::zShaderParameter& _Value)
{	
	auto paramMask	= mrShaderBinding.HAL()->mdCBufferParamMask[_Value.mhName];
	while( paramMask != 0 )
	{
		zUInt cbufferIdx = zenMath::BitsScan( paramMask );
		paramMask		^= (1<<cbufferIdx);
		marConstantBuffer[cbufferIdx]->SetValue(_Value);
	}
}

void GfxMeshStrip_DX11::SetResource(zHash32 _hResourceName, const GfxShaderResourceRef& _rResource, zU16 _uIndex )
{
	zcExp::ExportGfxShaderBinding::ShaderBindInfoIndex PerStageBindInfo;
	if( mrShaderBinding.HAL()->mdResourceBind.Get(_hResourceName, PerStageBindInfo) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			auto resIndex					= PerStageBindInfo.muShaderResIndex[stageIdx];
			const GfxShaderAnyRef&  rShader	= mrShaderBinding.HAL()->marShader[stageIdx];
			if( rShader.IsValid() && resIndex < rShader.HAL()->maResourceBinding.size() )
			{
				const zcExp::ExportGfxShader::ShaderBindInfo& BindInfo		= rShader->maResourceBinding[ resIndex ];
				zU8 uSlotOffset												= static_cast<zU8>(zenMath::Min<zU16>(_uIndex, BindInfo.muSlotCount));
				zU8 uSlotIndex												= BindInfo.muSlotIndex+uSlotOffset;
				//zenAssertMsg(uSlotIndex==_uIndex, "Invalid array index"); //!todo warning Error Message
				marShaderResources[stageIdx][BindInfo.meType][uSlotIndex]	= _rResource;
				mhShaderResourceStamp[stageIdx][BindInfo.meType]			= 0;
			}
		}
 	}
}

void GfxMeshStrip_DX11::SetResource(zHash32 _hResourceName, const zArray<GfxShaderResourceRef>& _arResources )
{
	zcExp::ExportGfxShaderBinding::ShaderBindInfoIndex PerStageBindInfo;
	if( mrShaderBinding.HAL()->mdResourceBind.Get(_hResourceName, PerStageBindInfo) )
	{
		for(zUInt stageIdx(0); stageIdx<zenConst::keShaderStage__Count; ++stageIdx)
		{
			auto resIndex					= PerStageBindInfo.muShaderResIndex[stageIdx];
			const GfxShaderAnyRef&  rShader	= mrShaderBinding.HAL()->marShader[stageIdx];
			if( rShader.IsValid() && resIndex < rShader.HAL()->maResourceBinding.size() )
			{
				const zcExp::ExportGfxShader::ShaderBindInfo& BindInfo				= rShader->maResourceBinding[ resIndex ];
				mhShaderResourceStamp[stageIdx][BindInfo.meType]					= 0;
				for( zU8 slotIdx(0), slotCount(zenMath::Min((zU8)_arResources.size(), BindInfo.muSlotCount)); slotIdx<slotCount; ++slotIdx )
					marShaderResources[stageIdx][BindInfo.meType][BindInfo.muSlotIndex+slotIdx] = _arResources[slotIdx];
			}
		}
 	}
}

//=================================================================================================
bool GfxShaderBinding_DX11::Initialize()
{
	for(zUInt idx(0); idx<zenConst::keShaderStage__Count; ++idx)
		marShader[idx] =  maShaderID[idx];
		
	return true;
}

void GfxShaderBinding_DX11::CreateShaderParam(zArrayDyn<zenRes::zGfxCBuffer>& Out_aShaderParam)const
{
	zUInt uValidShaderCount(0);
	zenRes::zGfxCBuffer arValidCBuffer[zcExp::kuDX11_CBufferPerStageMax];		
	for(zUInt idx(0), count(maCBufferParentID.size()); idx<count; ++idx)
	{
		if( IsSystemCBuffer(maCBufferParentID[idx], maResourceName[maCBufferParentBindIndex[idx]] ) == false )
			arValidCBuffer[uValidShaderCount++] = zcExp::CreateGfxCBuffer( maCBufferParentID[idx] );
	}
	Out_aShaderParam.Copy(arValidCBuffer, uValidShaderCount);
}

}
