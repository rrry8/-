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
	float cnt;			//タイム
	float screenSizeX;	//スクリーンサイズｘ
	float screenSizeY;	//スクリーンサイズｙ
	float loopNum;		//ループ回数
	float threshold;		//雲密度
}

float rand2(float2 uv, float seed)
{
	float2 s = float2(dot(uv, float2(127.1f, 311.7f)) + seed, dot(uv, float2(269.5f, 183.3f)) + seed);
	return -1.0f + 2.0f * frac(sin(s) * 43758.5453123f);
}

//パーリン
float4 perlin(float2 uv, float seed)
{
	//uv.y += cnt;
	float2 p = floor(uv);
	float2 f = frac(uv);
	//エルミート
	float2 u = f * f * (3.0f - 2.0f * f);

	//四隅
	float c00 = dot(rand2(p, seed), f);
	float c10 = dot(rand2(p + float2(1.0f, 0.0f), seed), f - float2(1.0f, 0.0f));
	float c01 = dot(rand2(p + float2(0.0f, 1.0f), seed), f - float2(0.0f, 1.0f));
	float c11 = dot(rand2(p + float2(1.0f, 1.0f), seed), f - float2(1.0f, 1.0f));

	float n = lerp(lerp(c00, c10, u.x), lerp(c01, c11, u.x), u.y);

	return float4(n, n, n, 1.0f);
}

float fbm(float2 uv)
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
	return f;
}

float CalcDensity(float3 pos,float2 uv)
{
	return threshold - (length(pos) + saturate(fbm(uv * 8.0f + float2(0.4f, 0.0f) * cnt) + 0.5f));
}

float3 CalcNormal(float3 pos, float2 radius)
{
	float v = 0.001f;
	return normalize(
		float3(
			CalcDensity(pos, radius) - CalcDensity(float3(pos.x - v, pos.y, pos.z), radius),
			CalcDensity(pos, radius) - CalcDensity(float3(pos.x, pos.y - v, pos.z), radius),
			CalcDensity(pos, radius) - CalcDensity(float3(pos.x, pos.y, pos.z - v), radius)
			)
	);
}

//テクスチャ
SamplerState sam       : register(s0);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	//定数
	//解像度
	const float2 resolusion = float2(screenSizeX, screenSizeY);

	//現在位置
	float2 nowPos = float2(input.uv.x * resolusion.x, input.uv.y * resolusion.y);
	float3 pos = float3((nowPos.xy * 2.0f - resolusion.xy) / min(resolusion.x, resolusion.y), 0.0f);

	//カメラ位置
	float3 cameraPos = camera.xyz;

	//レイ方向
	float3 rayDir = normalize(pos - cameraPos);
	float3 lightDir = normalize(float3(1.0f, -1.0f, 1.0f));//光の向かうベクトル(平行光線)
	//色
	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//z値
	float maxZ = 40.0f;
	float stepZ = maxZ / loopNum;

	int lightLoop = 6;
	//ライトｚ
	float maxZLight = 20.0f;
	float stepZLight = maxZLight / lightLoop;

	//透過率
	float trans = 1.0f;

	//光の吸収率
	float absorption = 20.0f;

	for (int i = 0; i < 64; i++)
	{
		//密度
		float density = CalcDensity(cameraPos,input.uv);

		//密度が0より大きい場合
		if (density > 0.0001f)
		{
			//透過率
			float tmp = density / loopNum;
			trans *= 1.0f - (tmp * absorption);
			//透過率が0より小さい場合は描画
			if (trans <= 0.01f)
			{
				break;
			}

			//色計算
			//不透明度
			float opacity = 50.0f;
			//最終的な係数
			float coeff = opacity * tmp * trans;
			//雲の色
			float cloudColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
			float4 diffuse = cloudColor * coeff;

			color += diffuse;
		}

		cameraPos += rayDir * stepZ;
	}
	//背景に近い位置は補間して自然にする
	float3 bg = lerp(float3(0.3f, 0.1f, 0.8f), input.diffuse, 1.0f - (input.uv.y + 1.0f) * 0.5f);
	color.rgb += bg;
	output.color = color;
	return output;
}
