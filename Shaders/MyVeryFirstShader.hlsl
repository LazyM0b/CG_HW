struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

cbuffer PositionW : register(b0)
{
	float4x4 posW;
};

//cbuffer PositionL : register(b1)
//{
//	float4x4 posL;
//}

PS_IN VSMain( VS_IN input)
{
	PS_IN output = (PS_IN)0;

	//float4x4 posM = mul(posW, posL);
	output.pos = mul(input.pos, posW);
	output.col = input.col;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = input.col;
//#ifdef TEST
//	if (input.pos.x > 400) col = TCOLOR;
//#endif
	return col;
}