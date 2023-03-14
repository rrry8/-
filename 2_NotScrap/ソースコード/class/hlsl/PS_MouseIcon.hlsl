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

//�萔
cbuffer buff : register(b5)
{
	float angle;
}

SamplerState sam;	//�T���v��
Texture2D tex : register(t0);
Texture2D norm : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);
	color *= input.diffuse;

	//���C�g��`
	float3 light = float3(cos(angle), sin(angle), 0.1f);
	light = normalize(light);
	float3 lightColor = float3(0.8f, 0.8f, 0.8f);

	//�@��
	float3 normal = norm.Sample(sam, input.uv).rgb;
	normal = (normal * 2) - 1;
	normal = normalize(normal);
	normal.z = -normal.z;

	//�g�U���ˌ�:
	float t = saturate(saturate(dot(normal, -light)) + 0.25f);

	color.rgb *= t;

	output.color = color;

	return output;
}