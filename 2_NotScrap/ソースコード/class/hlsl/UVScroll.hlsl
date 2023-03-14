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

cbuffer cbuf : register(b0)
{
	float threshold;
}

SamplerState sam;
Texture2D tex : register(t0);
Texture2D nor : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);


	output.color = color; //float3(1, 0, 0) * edge + color * (1.0 - edge);

	return output;
};