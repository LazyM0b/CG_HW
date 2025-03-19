struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSampler : SAMPLER : register(s0);

cbuffer PositionW : register(b0)
{
	float4x4 posW;
};

PS_IN VSMain( VS_IN input)
{
	PS_IN output = (PS_IN)0;

	output.pos = mul(input.pos, posW);
	output.col = input.col;
	output.tex = input.tex;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	
	float4 col = input.col;
	if (input.tex.x != 0.0f && input.tex.y != 0.0f)
		col = objTexture.Sample(objSampler, input.tex);

	return col;
}