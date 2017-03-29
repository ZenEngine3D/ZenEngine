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
	float4 position : SV_POSITION;
	float2 uv		: TEXCOORD;
};


cbuffer CBufferTest 
{    
    float4 vColor;
};


#if SHADER_VERTEX
	StructuredBuffer<float3> 	VInputPosition; 
	StructuredBuffer<float2>	VInputUV;

	PSInput VSMain( uint VertexId : SV_VertexID )
	{
		PSInput result;
		result.position = float4(VInputPosition[VertexId], 1);
		result.uv		= VInputUV[VertexId];
		return result;
	}

#elif SHADER_PIXEL
	Texture2D					g_texture;
	SamplerState				g_sampler;

	float4 PSMain(PSInput input) : SV_TARGET
	{
		return g_texture.Sample(g_sampler, input.uv) * float4(vColor.rgb, 1);
	}
#endif
