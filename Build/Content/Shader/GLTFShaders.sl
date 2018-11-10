//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

struct PSInput
{
	float2 UV		: TEXCOORD0;
	float3 Normal 	: TEXCOORD1;
	float4 Tangeant : TEXCOORD2;
	float4 Position : SV_POSITION;	
};


cbuffer CBufferTest 
{    
    float4 vColor;
};


#if SHADER_VERTEX

	cbuffer cbViewPhase : register( b1 )
	{
		matrix View;
		matrix Projection;
		matrix World;
	};

	StructuredBuffer<float3>	VInputPOSITION;
	float3 GetPosition( uint VtxId )
	{
		return VInputPOSITION[VtxId];
	}
	
	StructuredBuffer<float3> 	VInputNORMAL; 
	float3 GetNormal( uint VtxId )
	{
		return VInputNORMAL[VtxId];
	}
	
	StructuredBuffer<float4>	VInputTANGEANT;
	float4 GetTangeant( uint VtxId )
	{
		return VInputTANGEANT[VtxId];
	}
	
	StructuredBuffer<float2>	VInputTEXCOORD_0;
	float2 GetUV( uint VtxId )
	{
		return VInputTEXCOORD_0[VtxId];
	}
	
	PSInput VSMain( uint VertexId : SV_VertexID )
	{
		PSInput Output;
		
		Output.UV		= GetUV(VertexId);
		Output.Normal 	= GetNormal(VertexId);
		Output.Tangeant = GetTangeant(VertexId);
		
		matrix LocalToView	= mul(mul(World,View),Projection);
		Output.Position		= mul(float4(GetPosition(VertexId),1), LocalToView);
		return Output;
	}

#elif SHADER_PIXEL
	//Texture2D					g_texture;
	//SamplerState				g_sampler;

	float4 PSMain(PSInput Input) : SV_TARGET
	{
		return float4(Input.UV,1,1);// g_texture.Sample(g_sampler, input.UV) * float4(vColor.rgb, 1);
	}
#endif
