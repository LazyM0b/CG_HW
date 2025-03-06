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

cbuffer Transform : register(b0)
{
	float4 u;
	float4 v;
	float4 w;
	float4 Q;
};

PS_IN VSMain( VS_IN input)
{
	PS_IN output = (PS_IN)0;

	//Vector4 pos(input.pos);
	//Matrix posM(pos, Vector4.Zero, Vector4.Zero, Vector4.Zero);

	output.pos = float4(input.pos.x * u.x + input.pos.y * v.x + input.pos.z * w.x + input.pos.w * Q.x, 
						input.pos.x * u.y + input.pos.y * v.y + input.pos.z * w.y + input.pos.w * Q.y,
						input.pos.x * u.z + input.pos.y * v.z + input.pos.z * w.z + input.pos.w * Q.z,
						input.pos.x * u.w + input.pos.y * v.w + input.pos.z * w.w + input.pos.w * Q.w);
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