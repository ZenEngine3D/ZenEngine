#include "AllInclude.sl"

//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture_2D( txDiffuse );
Texture_2D( txColor );

struct TestStruct 
{
	uint 	Val1;
	uint 	Val2;
};

StructuredBuffer<float3> 			VInputPosition;
StructuredBuffer<SVertex_ColorUV>	VInputColorUV;
StructuredBuffer<float4> 			TestBufferUpdate;

cbuffer cbViewPhase : register( b1 )
{
    matrix View;
	matrix Projection;
	matrix World;
};

cbuffer cbMesh : register( b3 )
{    
    float4 vMeshColor;
};

cbuffer cbStrip : register( b5 )
{    
	float  vTest1;
	float2 vTest2;
	float3 vTest3;
    float4 vColor;
};

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float4 Color	: COLOR0;
    float2 UV 		: TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( uint VertexId : SV_VertexID )
{
    PS_INPUT output 				= (PS_INPUT)0;
	float3 bufferPos 				= VInputPosition[VtxInput_Offset+VertexId];
	SVertex_ColorUV bufferEntry 	= VInputColorUV[VtxInput_Offset+VertexId];
	
	matrix LocalToView				= mul(mul(World,View),Projection);
	output.Position					= mul(float4(bufferPos,1), LocalToView);
	output.Color					= UNorm4ToFloat4(bufferEntry.Color);
    output.UV						= bufferEntry.UV;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT VertexInput) : SV_Target
{
	float4 texColor = TextureSample2D( txColor, VertexInput.UV );
	//return float4(texColor.rgb*TestBufferUpdate[0].rgb, 1);	
	return float4(vColor.rgb*VertexInput.Color.rgb*texColor.rgb, VertexInput.Color.a );
	//return float4(VertexInput.Color.rgb*texColor.rgb, VertexInput.Color.a );
}

void PS2Output( PS_INPUT VertexInput, out float4 ColorOut1 : SV_Target0, out float4 ColorOut2 : SV_Target1) 
{
	float4 texColor = TextureSample2D( txColor, VertexInput.UV );
	ColorOut1 		= float4(vColor.rgb*texColor.rgb,1);
	ColorOut2 		= float4(VertexInput.UV.x, VertexInput.UV.y, 1, 1);
}
