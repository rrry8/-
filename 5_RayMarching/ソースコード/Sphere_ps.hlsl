struct PS_INPUT {
	float4 svpos	 : SV_POSITION;
	float4 diffuse   : COLOR0;
	float2 uv		 : TEXCOORD0;
};
//�o��
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

//�萔
cbuffer cBuffFloat4 : register(b5)
{
	float4 sphere;	//�~��� xyz:�ʒu w:���a
	float4 camera;	//�J�����ʒu xyz:�ʒu w:pad
}

cbuffer cBuffFloat : register(b6)
{
	float cnt;
	float screenSizeX;
	float screenSizeY;
	float loopNum;
}

float3 Duplicate(float3 pos)
{
	return fmod(pos, 6.0f) - 2.0f;
}

float CalcSphere(float3 pos, float radius)
{
	return length(pos) - radius;
}

float3 CalcNormal(float3 pos,float radius)
{
	float v = 0.001f;
	return normalize(
		float3(
			CalcSphere(pos, radius) - CalcSphere(float3(pos.x - v, pos.y, pos.z), radius),
			CalcSphere(pos, radius) - CalcSphere(float3(pos.x, pos.y - v, pos.z), radius),
			CalcSphere(pos, radius) - CalcSphere(float3(pos.x, pos.y, pos.z - v), radius)
			)
	);
}

//�e�N�X�`��
SamplerState sam       : register(s0);
Texture2D    tex       : register(t0);
Texture2D    ptn       : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	//�萔
	//�𑜓x
	const float2 resolusion = float2(screenSizeX, screenSizeY);
	const float radius = sphere.w;

	//���݈ʒu
	float2 nowPos = float2(input.uv.x * resolusion.x, input.uv.y * resolusion.y);
	float3 cameraPos = camera.xyz;

	//�ʒu
	float3 pos = float3((nowPos.xy * 2.0f - resolusion.xy) / min(resolusion.x, resolusion.y), 0.0f);
	float3 sphereColor = float3(0.2f, 1.0f, 0.7f);
	//pos.xy +=sphere.xy;
	//���C�g����
	float3 lightDir = normalize(float3(sin(cnt * 1.0f), cos(cnt * 1.0f), 1.0f));
	//���C�g�F
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	//���C����
	float3 rayDir = normalize(pos - cameraPos);

	//�F
	float3 color = float3(0.1f, 0.1f, 0.1f);

	//�A���r�G���g
	float3 ambientColor = float3(0.2f, 0.2f, 0.2f);

	for (int i = 0; i < 99; i++)
	{
		//�I�u�W�F�N�g�Ƃ̋���
		float distance = CalcSphere(cameraPos, radius);

		//�ڐG���Ă��邩����
		if (distance < 0.0001f)
		{
			//�f�B�t���[�Y
			float3 norm = CalcNormal(cameraPos, radius);
			float t = dot(norm, -lightDir);
			if (t < 0.0f)
			{
				t = 0.0f;
			}
			float diffuseColor = lightColor * t;

			//�X�y�L����
			float3 refVec = reflect(lightDir, norm);
			t = dot(refVec, -rayDir);
			if (t < 0.0f)
			{
				t = 0.0f;
			}
			t = pow(t, 20.0f);
			float3 specColor = lightColor * t;

			color = sphereColor * (diffuseColor + specColor + ambientColor);
			break;
		}
		cameraPos += rayDir * distance;
	}

	output.color = float4(color, 1.0f);
	return output;
}