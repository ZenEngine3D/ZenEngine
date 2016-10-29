#include "AllInclude.sl"

//Todo use header file shared between shader/engine
struct VS_INPUT 
{
	float2 	Position;
	float2	UV;
	uint  	Color;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};



StructuredBuffer<VS_INPUT>	VInputAll;
matrix 						ProjectionMatrix;
Texture_2D( txFont );

PS_INPUT VSMain(uint VertexId : SV_VertexID)
{	
	VS_INPUT input 	= VInputAll[VtxInput_Offset+VertexId];
	PS_INPUT output = (PS_INPUT)0;
	output.pos 		= mul( float4(input.Position, 0.f, 1.f), ProjectionMatrix ); 
	output.col 		= UNorm4ToFloat4(input.Color);
	output.uv  		= input.UV;	
	return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
	float4 out_col = input.col * TextureSample2D(txFont, input.uv); 
	return out_col; 
}