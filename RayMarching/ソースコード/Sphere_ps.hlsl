struct PS_INPUT {
	float4 svpos	 : SV_POSITION;
	float4 diffuse   : COLOR0;
	float2 uv		 : TEXCOORD0;
};
//出力
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

//定数
cbuffer cBuffFloat4 : register(b5)
{
	float4 sphere;	//円情報 xyz:位置 w:半径
	float4 camera;	//カメラ位置 xyz:位置 w:pad
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

//テクスチャ
SamplerState sam       : register(s0);
Texture2D    tex       : register(t0);
Texture2D    ptn       : register(t1);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	//定数
	//解像度
	const float2 resolusion = float2(screenSizeX, screenSizeY);
	const float radius = sphere.w;

	//現在位置
	float2 nowPos = float2(input.uv.x * resolusion.x, input.uv.y * resolusion.y);
	float3 cameraPos = camera.xyz;

	//位置
	float3 pos = float3((nowPos.xy * 2.0f - resolusion.xy) / min(resolusion.x, resolusion.y), 0.0f);
	float3 sphereColor = float3(0.2f, 1.0f, 0.7f);
	//pos.xy +=sphere.xy;
	//ライト方向
	float3 lightDir = normalize(float3(sin(cnt * 1.0f), cos(cnt * 1.0f), 1.0f));
	//ライト色
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	//レイ方向
	float3 rayDir = normalize(pos - cameraPos);

	//色
	float3 color = float3(0.1f, 0.1f, 0.1f);

	//アンビエント
	float3 ambientColor = float3(0.2f, 0.2f, 0.2f);

	for (int i = 0; i < 99; i++)
	{
		//オブジェクトとの距離
		float distance = CalcSphere(cameraPos, radius);

		//接触しているか判定
		if (distance < 0.0001f)
		{
			//ディフューズ
			float3 norm = CalcNormal(cameraPos, radius);
			float t = dot(norm, -lightDir);
			if (t < 0.0f)
			{
				t = 0.0f;
			}
			float diffuseColor = lightColor * t;

			//スペキュラ
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