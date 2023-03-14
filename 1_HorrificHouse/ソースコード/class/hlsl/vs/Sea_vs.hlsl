struct VS_INPUT
{
	float3 pos : POSITION;//���W
	float3 norm : NORMAL;//�@��
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 tangent:TANGENT;//�ڃx�N�g��
	float3 binormal:BINORMAL;//�]�@���x�N�g��
};

struct VS_OUTPUT
{
	float4 svpos:SV_POSITION;
	float3 pos:POSITION0;
	float4 diffuse:COLOR;
	float3 normal:NORMAL;
	float2 uv:TECOORD0;
	float3 viewVec : TEXCOORD1;
};

cbuffer BaseCBuffer : register(b1)
{
	matrix AntiViewportM;//4x4�r���[�|�[�g�t�s��
	matrix ProjectionM;//4x4�v���W�F�N�V�����s��
	float4x3 ViewM;//4x3(�r���[�s��)
	float4x3 LocalM;//4x3(��]�g�k���s�ړ�)
	float4		ToonOutLineSize;						// �g�D�[���̗֊s���̑傫��
	float		DiffuseSource;							// �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
	float		SpecularSource;						// �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
	float		MulSpecularColor;						// �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
	float		Padding;//�l�ߕ�(����)
}

cbuffer cBuff : register(b5)
{
	float cnt;			//�^�C��
	float gravity;		//�d��
	float pi;			//��
}

cbuffer cBuffFloat4 : register(b6)
{
	float4 cPos;	//�J�����ʒu(���[���h)
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT ret;
	float4 pos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	pos.xyz += input.pos;

	pos.xyz = mul(pos, LocalM);//���[���h
	ret.pos = pos;
	pos.xyz = mul(pos, ViewM);//�r���[
	pos = mul(pos, ProjectionM);//�v���W�F�N�V����

	ret.svpos = pos;

	ret.uv = input.uv0;
	ret.normal = normalize(input.norm);
	ret.diffuse = input.diffuse;
	ret.viewVec = cPos - ret.pos.xyz;

	return ret;
}