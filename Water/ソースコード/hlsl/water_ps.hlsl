// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 svpos:SV_POSITION;
	float3 pos:POSITION;
	float4 diffuse:COLOR;
	float3 normal:NORMAL;
	float2 uv:TECOORD0;
	float3 viewVec : TEXCOORD1;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 color           : SV_TARGET0;	// �F
};

// �萔�o�b�t�@�s�N�Z���V�F�[�_�[��{�p�����[�^
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
	float4		FactorColor;			// �A���t�@�l��

	float		MulAlphaColor;			// �J���[�ɃA���t�@�l����Z���邩�ǂ���( 0.0f:��Z���Ȃ�  1.0f:��Z���� )
	float		AlphaTestRef;			// �A���t�@�e�X�g�Ŏg�p�����r�l
	float2		Padding1;

	int			AlphaTestCmpMode;		// �A���t�@�e�X�g��r���[�h( DX_CMP_NEVER �Ȃ� )
	int3		Padding2;

	float4		IgnoreTextureColor;	// �e�N�X�`���J���[���������p�J���[
};

// ��{�p�����[�^
cbuffer cbD3D11_CONST_BUFFER_PS_BASE				: register(b1)
{
	DX_D3D11_PS_CONST_BUFFER_BASE		g_Base;
};

cbuffer cBuff : register(b5)
{
	float cnt;			//�^�C��
	float gravity;		//�d��
	float pi;			//��
}

cbuffer cBuffFloat4 : register(b6)
{
	float4 cPos;
}

float rand2(float2 uv, float seed)
{
	float2 s = float2(dot(uv, float2(127.1, 311.7)) + seed, dot(uv, float2(269.5, 183.3)) + seed);
	return -1 + 2 * frac(sin(s) * 43758.5453123);
}

//�p�[����
float4 perlin(float2 uv, float seed)
{
	//uv.y += cnt;
	float2 p = floor(uv);
	float2 f = frac(uv);
	//�G���~�[�g
	float2 u = f * f * (3.0f - 2.0f * f);

	//�l��
	float c00 = dot(rand2(p, seed), f);
	float c10 = dot(rand2(p + float2(1.0f, 0.0f), seed), f - float2(1.0f, 0.0f));
	float c01 = dot(rand2(p + float2(0.0f, 1.0f), seed), f - float2(0.0f, 1.0f));
	float c11 = dot(rand2(p + float2(1.0f, 1.0f), seed), f - float2(1.0f, 1.0f));

	float n = lerp(lerp(c00, c10, u.x), lerp(c01, c11, u.x), u.y);

	return float4(n, n, n, 1.0f);
}

//fbm�m�C�Y
float4 fbm(float2 uv)
{
	float f = 0;
	float2 q = uv;
	f += 0.5f * perlin(q, 0).x;
	q = q * 2.01f;
	f += 0.25f * perlin(q, 0).x;
	q = q * 2.02f;
	f += 0.125f * perlin(q, 0).x;
	q = q * 2.03f;
	f += 0.0625f * perlin(q, 0).x;
	q = q * 2.01f;
	f += 0.5f;
	return float4(f, f, f, 1);
}

//�@���v�Z
float3 CalcNormal(float2 uv)
{
	float origin = fbm(uv).x;
	float gradX = fbm(uv + float2(1, 0)).x;
	float gradY = fbm(uv + float2(0, 1)).x;

	float3 vecX = float3(1, 0, gradX - origin);
	float3 vecY = float3(0, 1, gradY - origin);

	float3 norm = cross(vecX, vecY);
	norm = normalize(norm);
	return norm;
}

//�t���l������
float Fresnel(float3 toCamera,float3 normal,float n1, float n2)
{
	//���܊֌W
	float A = n1 / n2;
	//���ˊp�� cos
	float B = dot(toCamera, normal);
	float C = sqrt(1.0f - A * A * (1.0f - B * B));

	float r1 = ((A * B - C) / (A * B + C));
	float r2 = ((A * C - B) / (A * C + B));

	//���˂̕���
	float ref = saturate(r1 * r1 + r2 * r2) / 2.0f;
	return saturate(ref);
}

SamplerState sam            : register(s0);	//�T���v��

//���I�L���[�u�e�N�X�`��
textureCUBE dynamicCubeTex : register(t0);

// main�֐�
PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	float3 normal = input.normal + CalcNormal(input.uv * 24)*0;

	//�J�����Ɍ������x�N�g��
	float3 viewVec = normalize(input.viewVec);

	//�L���[�u�p���˃x�N�g��
	float3 cubeRef = reflect(-viewVec, normal);

	//�L���[�u�J���[
	float4 cubeColor = dynamicCubeTex.Sample(sam, cubeRef);

	//�t���l��(�J�����ւ̃x�N�g��,�@��,���܊֌W)
	float fresnel = Fresnel(viewVec, normal, 1.000292f, 1.3334f);

	//���̐F
	float3 waterColor = float3(0.0f, 0.73f, 0.81f);

	//�ŏI�I�ȐF
	float3 finalColor = waterColor * 
		(1.0f - fresnel) + cubeColor.rgb 
		* fresnel;

	//���C�g�̐F
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	//���C�g�̕���
	float3 lightDir = normalize(float3(1.0f, -1.0f, 1.0f));

	//�f�B�t���[�Y
	float t = dot(normal, -lightDir);
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	float3 diffuse = lightColor * t;

	//�X�y�L����
	float3 refVec = reflect(lightDir, normal);
	float3 toEye = cPos.xyz - input.pos;
	toEye = normalize(toEye);
	t = dot(refVec, toEye);
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	t = pow(t, 3.0f);
	float3 specColor = lightColor * t;

	//���_�𔒂�����
	float topPower = 1.0f;
	float3 topColor = lightColor * pow(1.0f - abs(dot(viewVec, normal)), 20.0f) * topPower;

	float3 ambient = float3(0.5f, 0.5f, 0.5f);
	output.color = float4(finalColor * (diffuse + specColor + ambient+topColor), 1.0f);
	// �o�̓p�����[�^��Ԃ�
	return output;
}
