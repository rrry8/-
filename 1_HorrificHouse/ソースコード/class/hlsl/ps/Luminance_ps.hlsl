// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 svpos:SV_POSITION;
	float4 diffuse: COLOR;
	float2 uv : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color           : SV_TARGET0;	// �F
};

SamplerState sam            : register(s0);		//�T���v��
Texture2D    tex            : register(t0);		//�X�N���[��

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);

	//�P�x�v�Z
	float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

	//�P�x���Ⴂ���͕̂`�悵�Ȃ�
	clip(luminance - 0.95f);

	output.color = color;

	return output;
}
