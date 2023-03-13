struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float4 diffuse     : COLOR0;
	float2 uv  : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 color : SV_TARGET0;
};

SamplerState sam;
Texture2D screen : register(t0);
Texture2D noise : register(t1);

cbuffer ConstantBuffer : register(b5)
{
	float angle;
}

PS_OUTPUT main(PS_INPUT input)
{

	PS_OUTPUT output;

	//uv値を回転
	float2 uv = float2(
		input.uv.x + cos(angle) * 0.05f,
		input.uv.y + sin(angle) * 0.05f
		);

	//ノイズをサンプリング
	float4 p = noise.Sample(sam, uv);

	p.xy = (p.xy * 2.0 - 1.0f) * 0.05f;

	//背景をサンプリングして加算
	output.color = screen.Sample(sam, input.uv + p.xy);

	//徐々に透過
	output.color.a = input.diffuse.a;
	
	return output;
}
