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

	//uv�l����]
	float2 uv = float2(
		input.uv.x + cos(angle) * 0.05f,
		input.uv.y + sin(angle) * 0.05f
		);

	//�m�C�Y���T���v�����O
	float4 p = noise.Sample(sam, uv);

	p.xy = (p.xy * 2.0 - 1.0f) * 0.05f;

	//�w�i���T���v�����O���ĉ��Z
	output.color = screen.Sample(sam, input.uv + p.xy);

	//���X�ɓ���
	output.color.a = input.diffuse.a;
	
	return output;
}
