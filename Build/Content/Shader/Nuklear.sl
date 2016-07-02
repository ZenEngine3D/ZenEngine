#include "AllInclude.sl"

matrix ProjectionMatrix;
Texture_2D( txFont );

struct VS_INPUT
{
  float2 pos : POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
  float4 pos : SV_POSITION;
  float4 col : COLOR0;
  float2 uv  : TEXCOORD0;
};

PS_INPUT vs(VS_INPUT input)
{
  PS_INPUT output;
  output.pos = mul( float4(input.pos.xy, 0.f, 1.f), ProjectionMatrix );
  output.col = input.col;
  output.uv  = input.uv;
  return output;
}

float4 ps(PS_INPUT input) : SV_Target
{
  return input.col * TextureSample2D(txFont, input.uv);  
}
