struct PS_INPUT
{
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

//’è”
cbuffer buff : register(b0)
{
	float width;
}

SamplerState sam;	//ƒTƒ“ƒvƒ‰
Texture2D tex : register(t0);
Texture2D norm : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);

	if (width < input.uv.x && input.uv.x < width + 0.05f)
	{
		color.b = 0.0f;
	}

	output.color = color;

	return output;
}