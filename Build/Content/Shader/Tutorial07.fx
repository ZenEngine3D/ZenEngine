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
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	float Test : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
		
	matrix LocalToView	= mul(mul(World,View),Projection);
	output.Pos			= mul( input.Pos, LocalToView );
	
    output.Tex			= input.Tex;
    //output.Test			= vMeshColor.a;
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input) : SV_Target
{
	float4 texColor = TextureSample2D( txColor, input.Tex );
	return float4(vColor.rgb*texColor.rgb,1);
}
